@echo off
setlocal enabledelayedexpansion

REM Compilar y ejecutar TaskScript
cd /d "%~dp0"

REM Inyectar PATH para MinGW
set PATH=C:\Qt\Tools\mingw1120_64\bin;C:\Qt\6.5.3\mingw_64\bin;%PATH%

echo Compilando...
C:\Qt\Tools\CMake_64\bin\cmake.exe -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=C:\Qt\Tools\mingw1120_64\bin\g++.exe -DCMAKE_PREFIX_PATH=C:\Qt\6.5.3\mingw_64 >nul 2>&1
C:\Qt\Tools\CMake_64\bin\cmake.exe --build build -j >nul 2>&1

rem Allow optional .task file as first argument
set "INPUT=test\test04_error_sintactico.task"
if not "%~1"=="" set "INPUT=%~1"

if %ERRORLEVEL% equ 0 (
    echo [OK] Compilacion exitosa
    echo Ejecutando... %INPUT%
    rem If user requested gui, run the minimal Qt test window
    if /I "%INPUT%"=="gui" (
        start "" .\build\taskscript_qt_test.exe
    ) else (
        .\build\taskscript_cli.exe "%INPUT%"
    )
    echo.
    if /I not "%INPUT%"=="gui" (
        echo [OK] Reportes generados en ./output/
        if exist "output\tokens.html" (
            start "" "output\tokens.html"
        ) else (
            explorer "output"
        )
    )
) else (
    echo [ERROR] Fallo en compilacion
    pause
)
