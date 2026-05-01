# Script de DIAGNOSTICO - verifica que funciona
# Uso: .\DIAGNOSTICO.ps1

Write-Host "================================" -ForegroundColor Cyan
Write-Host "DIAGNOSTICO - TaskScript" -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan

$projectDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $projectDir

Write-Host "`n[1/5] Verificando archivos necesarios..." -ForegroundColor Yellow

$filesToCheck = @(
    "build/taskscript_cli.exe",
    "build/taskscript_qt_test.exe",
    "build/taskscript_gui.exe",
    "src/app/main.cpp",
    "src/core/report/Report.h",
    "test/test06_formato_usuario.task"
)

foreach ($file in $filesToCheck) {
    $exists = Test-Path $file
    $status = if ($exists) { "OK" } else { "FALTA" }
    $color = if ($exists) { "Green" } else { "Red" }
    Write-Host "  [$status] $file" -ForegroundColor $color
}

Write-Host "`n[2/5] Verificando compilacion..." -ForegroundColor Yellow
if (Test-Path "build/CMakeCache.txt") {
    Write-Host "  [OK] CMake configurado" -ForegroundColor Green
} else {
    Write-Host "  [FALTA] CMake no configurado - ejecuta: .\COMPILAR.ps1" -ForegroundColor Red
}

Write-Host "`n[3/5] Archivos generados previamente..." -ForegroundColor Yellow
if (Test-Path "output") {
    $htmlFiles = Get-ChildItem "output/*.html" 2>$null
    if ($htmlFiles) {
        Write-Host "  [OK] Se encontraron $($htmlFiles.Count) reportes HTML" -ForegroundColor Green
        foreach ($file in $htmlFiles) {
            $sizeKb = [math]::Round($file.Length / 1KB, 1)
            Write-Host "    - $($file.Name) ($sizeKb KB)" -ForegroundColor Green
        }
    } else {
        Write-Host "  [VACIO] output/ existe pero sin reportes" -ForegroundColor Yellow
    }
} else {
    Write-Host "  [VACIO] Carpeta output/ no existe (se creara al ejecutar)" -ForegroundColor Yellow
}

Write-Host "`n[4/5] Informacion de Qt..." -ForegroundColor Yellow
$qtPath = "C:/Qt/6.5.3/mingw_64"
if (Test-Path $qtPath) {
    Write-Host "  [OK] Qt 6.5.3 encontrado" -ForegroundColor Green
} else {
    Write-Host "  [FALTA] Qt no encontrado en $qtPath" -ForegroundColor Red
}

Write-Host "`n[5/5] Ejecutando CLI para diagnostico..." -ForegroundColor Yellow

if (Test-Path "build/taskscript_cli.exe") {
    Write-Host "`nEjecutando CLI con test06_formato_usuario.task..." -ForegroundColor Yellow
    Write-Host "========================================" -ForegroundColor Gray

    $cliLog = Join-Path $projectDir "output\cli_run.log"
    $cmdLine = "build\taskscript_cli.exe test\test06_formato_usuario.task > `"$cliLog`" 2>&1"
    $process = Start-Process cmd.exe -ArgumentList "/c", $cmdLine -WorkingDirectory $projectDir -PassThru -Wait -NoNewWindow
    $exitCode = $process.ExitCode

    $output = if (Test-Path $cliLog) { Get-Content $cliLog -Raw } else { "" }

    if ($output) {
        Write-Host $output -ForegroundColor Gray
    }

    Write-Host "========================================" -ForegroundColor Gray
    Write-Host "Exit code: $exitCode" -ForegroundColor Yellow

    Write-Host "`nVerificando reportes generados..." -ForegroundColor Yellow
    $htmlFiles = Get-ChildItem "output/*.html" 2>$null
    if ($htmlFiles) {
        Write-Host "[OK] Se generaron $($htmlFiles.Count) reportes:" -ForegroundColor Green
        foreach ($file in $htmlFiles) {
            Write-Host "  - $($file.Name)" -ForegroundColor Green
        }

        Write-Host "`nPuedes abrir los reportes con:" -ForegroundColor Cyan
        Write-Host "  - start output/errors.html      (errores)" -ForegroundColor Cyan
        Write-Host "  - start output/tokens.html      (tokens)" -ForegroundColor Cyan
        Write-Host "  - start output/stats.html       (estadisticas)" -ForegroundColor Cyan
        Write-Host "  - start output/workload.html    (carga de trabajo)" -ForegroundColor Cyan
    } else {
        Write-Host "[ERROR] No se generaron reportes. Hay un problema en la CLI." -ForegroundColor Red
    }
} else {
    Write-Host "[ERROR] build/taskscript_cli.exe no encontrado" -ForegroundColor Red
    Write-Host "Compila primero: .\COMPILAR.ps1" -ForegroundColor Red
}

Write-Host "`n================================" -ForegroundColor Cyan
Write-Host "Diagnostico completado." -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan
