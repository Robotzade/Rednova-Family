param(
    [Parameter(Mandatory = $true)][string]$ExpectedVid,
    [Parameter(Mandatory = $true)][string]$ExpectedAppPid,
    [Parameter(Mandatory = $true)][string]$ExpectedBootPid,
    [Parameter(Mandatory = $true)][string]$ExpectedBoard,
    [Parameter(Mandatory = $true)][ValidateSet("V2", "MICRO")][string]$ExpectedModel,
    [Parameter(Mandatory = $true)][string]$IdentityFile,
    [Parameter(Mandatory = $true)][string]$Port,
    [Parameter(Mandatory = $true)][string]$Avrdude,
    [Parameter(Mandatory = $true)][string]$Config,
    [Parameter(Mandatory = $true)][string]$Mcu,
    [Parameter(Mandatory = $true)][string]$Protocol,
    [Parameter(Mandatory = $true)][string]$Speed,
    [Parameter(Mandatory = $true)][string]$HexFile,
    [string]$UploadVerbose = "",
    [string]$UploadVerify = ""
)

$ErrorActionPreference = "Stop"
$UploadVerbose = $UploadVerbose.Substring(1)
$UploadVerify = $UploadVerify.Substring(1)
$expectedAppHardwareId = "VID_$($ExpectedVid.ToUpperInvariant())&PID_$($ExpectedAppPid.ToUpperInvariant())"
$expectedBootHardwareId = "VID_$($ExpectedVid.ToUpperInvariant())&PID_$($ExpectedBootPid.ToUpperInvariant())"
$legacyAppHardwareIds = @("VID_2341&PID_8036", "VID_2A03&PID_8036")
$legacyBootHardwareIds = @("VID_2341&PID_0036", "VID_2A03&PID_0036")
$identityOffset = 1016
$identityV2 = [byte[]](0x52, 0x44, 0x4E, 0x56, 0x01, 0x02, 0xFD, 0xA5)
$identityMicro = [byte[]](0x52, 0x44, 0x4E, 0x56, 0x01, 0x03, 0xFC, 0xA5)
$expectedIdentity = if ($ExpectedModel -eq "V2") { $identityV2 } else { $identityMicro }
$otherModel = if ($ExpectedModel -eq "V2") { "Rednova Micro" } else { "Rednova V2" }

function Test-HardwareId([string]$PnpDeviceId, [string[]]$HardwareIds) {
    foreach ($hardwareId in $HardwareIds) {
        if ($PnpDeviceId -match [regex]::Escape($hardwareId)) {
            return $true
        }
    }
    return $false
}

function Get-SerialDevice([string]$DeviceId) {
    return Get-CimInstance Win32_SerialPort |
        Where-Object { $_.DeviceID -eq $DeviceId } |
        Select-Object -First 1
}

function Test-ByteArray([byte[]]$Actual, [byte[]]$Expected) {
    if ($Actual.Length -ne $Expected.Length) { return $false }
    for ($index = 0; $index -lt $Actual.Length; $index++) {
        if ($Actual[$index] -ne $Expected[$index]) { return $false }
    }
    return $true
}

$serialDevice = Get-SerialDevice $Port
if ($null -eq $serialDevice) {
    [Console]::Error.WriteLine("Rednova upload blocked: $Port could not be identified.")
    exit 20
}

$isRednovaApplication = $serialDevice.PNPDeviceID -match [regex]::Escape($expectedAppHardwareId)
$isRednovaBootloader = $serialDevice.PNPDeviceID -match [regex]::Escape($expectedBootHardwareId)
$isLegacyApplication = Test-HardwareId $serialDevice.PNPDeviceID $legacyAppHardwareIds
$isLegacyBootloader = Test-HardwareId $serialDevice.PNPDeviceID $legacyBootHardwareIds
$isApplication = $isRednovaApplication -or $isLegacyApplication
$isBootloader = $isRednovaBootloader -or $isLegacyBootloader

if (-not $isApplication -and -not $isBootloader) {
    [Console]::Error.WriteLine(@"
Rednova upload blocked: wrong board selected.
Selected board : $ExpectedBoard
Expected USB   : $expectedAppHardwareId or $expectedBootHardwareId
Legacy USB     : Leonardo VID_2341/VID_2A03, PID_8036/PID_0036
Connected USB  : $($serialDevice.PNPDeviceID)
Select the physical Rednova model connected to $Port and try again.
"@)
    exit 21
}

$uploadPort = $Port
if ($isApplication) {
    try {
        $touch = New-Object System.IO.Ports.SerialPort $Port, 1200, ([System.IO.Ports.Parity]::None), 8, ([System.IO.Ports.StopBits]::One)
        $touch.DtrEnable = $false
        $touch.Open()
        $touch.Close()
        $touch.Dispose()
    } catch {
        [Console]::Error.WriteLine("Rednova upload blocked: 1200-bps reset failed on $Port. $($_.Exception.Message)")
        exit 22
    }

    $bootDevice = $null
    for ($attempt = 0; $attempt -lt 40 -and $null -eq $bootDevice; $attempt++) {
        Start-Sleep -Milliseconds 250
        $bootDevice = Get-CimInstance Win32_SerialPort |
            Where-Object {
                ($_.PNPDeviceID -match [regex]::Escape($expectedBootHardwareId)) -or
                (Test-HardwareId $_.PNPDeviceID $legacyBootHardwareIds)
            } |
            Select-Object -First 1
    }

    if ($null -eq $bootDevice) {
        [Console]::Error.WriteLine("Rednova upload blocked: $ExpectedBoard or legacy Leonardo bootloader port was not found.")
        exit 23
    }
    $uploadPort = $bootDevice.DeviceID
}

# The USB product string belongs to the currently running sketch and can be
# changed by an Arduino Leonardo upload. The EEPROM identity is independent of
# that sketch, so it is the authoritative physical-model lock.
$eepromDump = Join-Path ([System.IO.Path]::GetTempPath()) ("rednova-eeprom-" + [guid]::NewGuid().ToString("N") + ".bin")
try {
    & $Avrdude "-C$Config" "-p$Mcu" "-c$Protocol" "-P$uploadPort" "-b$Speed" "-Ueeprom:r:$($eepromDump):r"
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $eepromDump)) {
        [Console]::Error.WriteLine("Rednova upload blocked: the permanent model identity could not be read.")
        exit 24
    }

    $eeprom = [System.IO.File]::ReadAllBytes($eepromDump)
    if ($eeprom.Length -lt ($identityOffset + 8)) {
        [Console]::Error.WriteLine("Rednova upload blocked: the EEPROM identity area is unavailable.")
        exit 24
    }

    $actualIdentity = [byte[]]$eeprom[$identityOffset..($identityOffset + 7)]
    $blankIdentity = Test-ByteArray $actualIdentity ([byte[]](0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF))

    if ($blankIdentity) {
        # Native Rednova USB identity proves the model. For an unprovisioned
        # Leonardo-era board, the explicitly selected Old Boot entry performs
        # the one-time model claim. Every later upload must match this identity.
        $canProvisionFromNativeIdentity = ($ExpectedVid -eq "1209") -and $isRednovaApplication
        $canProvisionFromOldBootSelection = ($ExpectedVid -eq "2341") -and $isLegacyApplication
        if (-not $canProvisionFromNativeIdentity -and -not $canProvisionFromOldBootSelection) {
            [Console]::Error.WriteLine(@"
Rednova upload blocked: this board has no permanent model identity and cannot
be safely identified from the selected port. Use the correct Old Boot Leonardo
model selection for the first upload, or use Burn Bootloader with Arduino as ISP.
"@)
            exit 25
        }

        & $Avrdude "-C$Config" "-p$Mcu" "-c$Protocol" "-P$uploadPort" "-b$Speed" "-Ueeprom:w:$($IdentityFile):i"
        if ($LASTEXITCODE -ne 0) {
            [Console]::Error.WriteLine("Rednova upload blocked: model identity initialization failed.")
            exit 26
        }
        [Console]::Error.WriteLine("Rednova model identity initialized as $ExpectedModel. Future uploads are locked to this model.")
    } elseif (-not (Test-ByteArray $actualIdentity $expectedIdentity)) {
        if ((Test-ByteArray $actualIdentity $identityV2) -or (Test-ByteArray $actualIdentity $identityMicro)) {
            [Console]::Error.WriteLine("Rednova upload blocked: this board is permanently identified as $otherModel, not $ExpectedBoard.")
        } else {
            [Console]::Error.WriteLine("Rednova upload blocked: the EEPROM model identity is invalid. Use the factory ISP recovery procedure.")
        }
        exit 27
    }
} finally {
    if (Test-Path -LiteralPath $eepromDump) {
        Remove-Item -LiteralPath $eepromDump -Force
    }
}

# Caterina starts the application when the EEPROM-read avrdude session closes.
# Enter the bootloader once more for the actual flash write.
$applicationAfterIdentity = $null
for ($attempt = 0; $attempt -lt 40 -and $null -eq $applicationAfterIdentity; $attempt++) {
    Start-Sleep -Milliseconds 250
    $applicationAfterIdentity = Get-SerialDevice $Port
}
if ($null -eq $applicationAfterIdentity) {
    [Console]::Error.WriteLine("Rednova upload blocked: application port $Port did not return after model verification.")
    exit 28
}

try {
    $touch = New-Object System.IO.Ports.SerialPort $Port, 1200, ([System.IO.Ports.Parity]::None), 8, ([System.IO.Ports.StopBits]::One)
    $touch.DtrEnable = $false
    $touch.Open()
    $touch.Close()
    $touch.Dispose()
} catch {
    [Console]::Error.WriteLine("Rednova upload blocked: second 1200-bps reset failed on $Port. $($_.Exception.Message)")
    exit 28
}

$bootDevice = $null
for ($attempt = 0; $attempt -lt 40 -and $null -eq $bootDevice; $attempt++) {
    Start-Sleep -Milliseconds 250
    $bootDevice = Get-CimInstance Win32_SerialPort |
        Where-Object {
            ($_.PNPDeviceID -match [regex]::Escape($expectedBootHardwareId)) -or
            (Test-HardwareId $_.PNPDeviceID $legacyBootHardwareIds)
        } |
        Select-Object -First 1
}
if ($null -eq $bootDevice) {
    [Console]::Error.WriteLine("Rednova upload blocked: bootloader did not return after model verification.")
    exit 28
}
$uploadPort = $bootDevice.DeviceID

$avrdudeArguments = @("-C$Config")
if ($UploadVerbose) {
    $avrdudeArguments += $UploadVerbose.Split(" ", [System.StringSplitOptions]::RemoveEmptyEntries)
}
if ($UploadVerify) {
    $avrdudeArguments += $UploadVerify.Split(" ", [System.StringSplitOptions]::RemoveEmptyEntries)
}
$avrdudeArguments += @(
    "-p$Mcu",
    "-c$Protocol",
    "-P$uploadPort",
    "-b$Speed",
    "-D",
    "-Uflash:w:$($HexFile):i"
)

& $Avrdude @avrdudeArguments
exit $LASTEXITCODE
