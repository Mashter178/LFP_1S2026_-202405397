# Script para compilar Proyecto 2 en DEBUG
# Uso: .\COMPILAR_DEBUG.ps1

Write-Host "================================" -ForegroundColor Cyan
Write-Host "Compilando Proyecto 2 (DEBUG)" -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan

$projectDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $projectDir

$cmakePath = "C:/Qt/Tools/CMake_64/bin/cmake.exe"
if (-not (Test-Path $cmakePath)) {
    Write-Host "ERROR: CMake no encontrado en $cmakePath" -ForegroundColor Red
    exit 1
}

# Eliminar build anterior
if (Test-Path "build") {
    Write-Host "`n[1/4] Limpiando build anterior..." -ForegroundColor Yellow
    Remove-Item build -Recurse -Force
}

Write-Host "`n[2/4] Configurando CMake en DEBUG..." -ForegroundColor Yellow
& $cmakePath -S . -B build -G "MinGW Makefiles" `
    -DCMAKE_BUILD_TYPE=Debug `
    -DCMAKE_CXX_COMPILER=C:/Qt/Tools/mingw1120_64/bin/g++.exe `
    -DCMAKE_PREFIX_PATH=C:/Qt/6.5.3/mingw_64

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Fallo al configurar CMake" -ForegroundColor Red
    exit 1
}

Write-Host "`n[3/4] Compilando proyecto en DEBUG..." -ForegroundColor Yellow
& $cmakePath --build build -j --config Debug

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Fallo la compilacion" -ForegroundColor Red
    exit 1
}

Write-Host "`n[4/4] Compilacion DEBUG completada" -ForegroundColor Green

if (Test-Path "build/taskscript_cli.exe") {
    Write-Host "`n[OK] CLI compilado: build/taskscript_cli.exe" -ForegroundColor Green
}

if (Test-Path "build/taskscript_qt_test.exe") {
    Write-Host "[OK] GUI compilada: build/taskscript_qt_test.exe" -ForegroundColor Green
}

if (Test-Path "build/taskscript_gui.exe") {
    Write-Host "[OK] GUI real compilada: build/taskscript_gui.exe" -ForegroundColor Green
}

Write-Host "`nProximo paso: ejecuta .\EJECUTAR.ps1 para correr la GUI" -ForegroundColor Cyan
