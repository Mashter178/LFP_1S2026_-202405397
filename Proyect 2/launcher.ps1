# Simple launcher with file picker for TaskScript files
$projectPath = Split-Path -Parent $MyInvocation.MyCommandPath
Set-Location $projectPath

Add-Type -AssemblyName System.Windows.Forms
$ofd = New-Object System.Windows.Forms.OpenFileDialog
$ofd.Filter = "TaskScript files (*.task)|*.task|All files (*.*)|*.*"
$ofd.InitialDirectory = Join-Path $projectPath "test"
$ofd.Multiselect = $false

if ($ofd.ShowDialog() -eq [System.Windows.Forms.DialogResult]::OK) {
    $file = $ofd.FileName
    Write-Host "Seleccionado: $file"
    & .\build.bat $file
} else {
    Write-Host "Ningún archivo seleccionado. Saliendo."
}
