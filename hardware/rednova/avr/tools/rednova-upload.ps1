param(
    [Parameter(Mandatory = $true)][string]$ExpectedVid,
    [Parameter(Mandatory = $true)][string]$ExpectedAppPid,
    [Parameter(Mandatory = $true)][string]$ExpectedBootPid,
    [Parameter(Mandatory = $true)][string]$ExpectedBoard,
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
