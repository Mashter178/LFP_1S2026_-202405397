# Script para compilar Proyecto 2 (TaskScript)
# Uso: .\COMPILAR.ps1

Write-Host "================================" -ForegroundColor Cyan
Write-Host "Compilando Proyecto 2 (TaskScript)" -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan

$projectDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $projectDir

$cmakePath = "C:/Qt/Tools/CMake_64/bin/cmake.exe"
if (-not (Test-Path $cmakePath)) {
    Write-Host "ERROR: CMake no encontrado en $cmakePath" -ForegroundColor Red
    exit 1
}

if (-not (Test-Path "build")) {
    Write-Host "`n[1/3] Configurando CMake..." -ForegroundColor Yellow
    & $cmakePath -S . -B build -G "MinGW Makefiles" `
        -DCMAKE_CXX_COMPILER=C:/Qt/Tools/mingw1120_64/bin/g++.exe `
        -DCMAKE_PREFIX_PATH=C:/Qt/6.5.3/mingw_64

    if ($LASTEXITCODE -ne 0) {
        Write-Host "ERROR: Fallo al configurar CMake" -ForegroundColor Red
        exit 1
    }
}

Write-Host "`n[2/3] Compilando proyecto..." -ForegroundColor Yellow
& $cmakePath --build build -j

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Fallo la compilacion" -ForegroundColor Red
    exit 1
}

Write-Host "`n[3/3] Compilacion completada" -ForegroundColor Green

if (Test-Path "build/taskscript_cli.exe") {
    Write-Host "`n[OK] Ejecutable CLI: build/taskscript_cli.exe" -ForegroundColor Green
}

if (Test-Path "build/taskscript_qt_test.exe") {
    Write-Host "[OK] Ejecutable GUI de prueba: build/taskscript_qt_test.exe" -ForegroundColor Green
} elseif (Test-Path "build/taskscript_gui.exe") {
    Write-Host "[OK] Ejecutable GUI: build/taskscript_gui.exe" -ForegroundColor Green
}

Write-Host "`nProximo paso: ejecuta .\EJECUTAR.ps1 para correr la ventana Qt" -ForegroundColor Cyan
