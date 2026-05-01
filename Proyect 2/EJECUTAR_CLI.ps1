# Script para ejecutar CLI directamente (sin GUI)
# Uso: .\EJECUTAR_CLI.ps1 [archivo.task]
# Ejemplo: .\EJECUTAR_CLI.ps1 test\test06_formato_usuario.task

param(
    [string]$filePath = "test/test06_formato_usuario.task"
)

Write-Host "================================" -ForegroundColor Cyan
Write-Host "Ejecutando TaskScript CLI" -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan

$projectDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $projectDir

if (-not (Test-Path "build/taskscript_cli.exe")) {
    Write-Host "ERROR: CLI no compilada. Ejecuta primero: .\COMPILAR.ps1" -ForegroundColor Red
    exit 1
}

if (-not (Test-Path $filePath)) {
    Write-Host "ERROR: Archivo no encontrado: $filePath" -ForegroundColor Red
    Write-Host "`nArchivos test disponibles:" -ForegroundColor Yellow
    Get-ChildItem "test/*.task" | ForEach-Object { Write-Host "  - $($_.Name)" -ForegroundColor Yellow }
    exit 1
}

if (Test-Path "output") {
    Remove-Item "output/*" -Force -ErrorAction SilentlyContinue
}

Write-Host "`n[1/3] Procesando: $filePath" -ForegroundColor Yellow

$cliLog = Join-Path $projectDir "output\cli_run.log"
$cmdLine = "build\taskscript_cli.exe $filePath > `"$cliLog`" 2>&1"
$process = Start-Process cmd.exe -ArgumentList "/c", $cmdLine -WorkingDirectory $projectDir -PassThru -Wait -NoNewWindow
$exitCode = $process.ExitCode

$cliOutput = if (Test-Path $cliLog) { Get-Content $cliLog -Raw } else { "" }

if ($cliOutput) {
    Write-Host $cliOutput -ForegroundColor Gray
}

if ($exitCode -ne 0) {
    Write-Host "ERROR: Fallo al procesar el archivo (code: $exitCode)" -ForegroundColor Red
    exit $exitCode
}

Write-Host "`n[2/3] Analisis completado" -ForegroundColor Green

Write-Host "`n[3/3] Reportes generados:" -ForegroundColor Green
$htmlFiles = @(
    "output/tokens.html",
    "output/errors.html",
    "output/stats.html",
    "output/kanban.html",
    "output/workload.html"
)

foreach ($file in $htmlFiles) {
    if (Test-Path $file) {
        $size = (Get-Item $file).Length
        $sizeKb = [math]::Round($size / 1KB, 1)
        Write-Host "  - $(Split-Path $file -Leaf) ($sizeKb KB)" -ForegroundColor Green
    }
}

Write-Host "`nPara abrir los reportes en el navegador:" -ForegroundColor Cyan
Write-Host "  Start-Process output/errors.html" -ForegroundColor Cyan
Write-Host "  Start-Process output/kanban.html" -ForegroundColor Cyan
Write-Host "  Start-Process output/workload.html" -ForegroundColor Cyan
Write-Host "  Start-Process output/stats.html" -ForegroundColor Cyan
Write-Host "  Start-Process output/tokens.html" -ForegroundColor Cyan

Write-Host "`nTodos los reportes estan en: ./output/" -ForegroundColor Cyan
