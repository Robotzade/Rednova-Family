param(
    [Parameter(Mandatory = $true)][string]$ExpectedPid,
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
$expectedHardwareId = "VID_1209&PID_$($ExpectedPid.ToUpperInvariant())"
$serialDevice = $null

for ($attempt = 0; $attempt -lt 20 -and $null -eq $serialDevice; $attempt++) {
    $serialDevice = Get-CimInstance Win32_SerialPort |
        Where-Object { $_.DeviceID -eq $Port } |
        Select-Object -First 1

    if ($null -eq $serialDevice) {
        Start-Sleep -Milliseconds 250
    }
}

if ($null -eq $serialDevice) {
    [Console]::Error.WriteLine("Rednova upload blocked: $Port could not be identified.")
    exit 20
}

if ($serialDevice.PNPDeviceID -notmatch [regex]::Escape($expectedHardwareId)) {
    [Console]::Error.WriteLine(@"
Rednova upload blocked: wrong board selected.
Selected board : $ExpectedBoard
Expected USB   : $expectedHardwareId
Connected USB  : $($serialDevice.PNPDeviceID)
Select the physical Rednova model connected to $Port and try again.
"@)
    exit 21
}

$avrdudeArguments = @(
    "-C$Config"
)

if ($UploadVerbose) {
    $avrdudeArguments += $UploadVerbose.Split(" ", [System.StringSplitOptions]::RemoveEmptyEntries)
}
if ($UploadVerify) {
    $avrdudeArguments += $UploadVerify.Split(" ", [System.StringSplitOptions]::RemoveEmptyEntries)
}

$avrdudeArguments += @(
    "-p$Mcu",
    "-c$Protocol",
    "-P$Port",
    "-b$Speed",
    "-D",
    "-Uflash:w:$($HexFile):i"
)

& $Avrdude @avrdudeArguments
exit $LASTEXITCODE
