# Script para ejecutar la interfaz grafica de TaskScript.
# Uso: .\EJECUTAR.ps1

Write-Host "================================" -ForegroundColor Cyan
Write-Host "Ejecutando TaskScript GUI" -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan

$projectDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $projectDir

$guiCandidates = @(
    "build/taskscript_gui.exe",
    "build/taskscript_qt_test.exe"
)

$guiExe = $null
foreach ($candidate in $guiCandidates) {
    if (Test-Path $candidate) {
        $guiExe = $candidate
        break
    }
}

if (-not $guiExe) {
    Write-Host "ERROR: No se encontro un ejecutable GUI." -ForegroundColor Red
    Write-Host "Ejecuta primero .\COMPILAR.ps1 y revisa el target Qt." -ForegroundColor Red
    exit 1
}

Write-Host "`n[1/2] Iniciando GUI..." -ForegroundColor Yellow
Write-Host "Se abrira la GUI real de TaskScript." -ForegroundColor Yellow

$process = Start-Process $guiExe -PassThru -WindowStyle Normal

Write-Host "`n[2/2] GUI ejecutandose (PID: $($process.Id))" -ForegroundColor Green
Write-Host "`nInstrucciones:" -ForegroundColor Cyan
Write-Host "  - Selecciona un archivo .task y usa Analizar o Exportar y abrir HTML." -ForegroundColor Cyan
Write-Host "  - Si solo ves la ventana de prueba, revisa que exista build/taskscript_gui.exe." -ForegroundColor Cyan

Write-Host "`nEsperando a que cierres la GUI..." -ForegroundColor Yellow
$process.WaitForExit()

Write-Host "`nGUI cerrada. Reportes disponibles en: ./output/" -ForegroundColor Green
