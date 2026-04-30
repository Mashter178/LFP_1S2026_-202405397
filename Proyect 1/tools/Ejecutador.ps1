param(
    [ValidateSet('gui','cli','build','configure')]
    [string]$Mode = 'gui',
    [string]$MedFile = 'test/hospital_valido_01.med'
)

$ErrorActionPreference = 'Stop'

$projectRoot = Split-Path -Parent $PSScriptRoot
$cmake = 'C:/Qt/Tools/CMake_64/bin/cmake.exe'
$compiler = 'C:/Qt/Tools/mingw1120_64/bin/g++.exe'
$qtPrefix = 'C:/Qt/6.5.3/mingw_64'
$qtBin = Join-Path $qtPrefix 'bin'
$qtPlugins = Join-Path $qtPrefix 'plugins'
$qtPlatformPlugins = Join-Path $qtPlugins 'platforms'
$primaryBuildDir = Join-Path $projectRoot 'build'
$altBuildDir = Join-Path $projectRoot 'build_alt'
$guiExeName = 'medlang_gui.exe'
$cliExeName = 'medlang_cli.exe'

Set-Location $projectRoot

$env:PATH = "$qtBin;$env:PATH"
$env:QT_PLUGIN_PATH = $qtPlugins
$env:QT_QPA_PLATFORM_PLUGIN_PATH = $qtPlatformPlugins

function Test-BuildConfigured {
    param(
        [string]$BuildDir
    )

    $cacheFile = Join-Path $BuildDir 'CMakeCache.txt'
    $makeFile = Join-Path $BuildDir 'Makefile'
    return (Test-Path $cacheFile) -and (Test-Path $makeFile)
}

function Invoke-Configure {
    param(
        [string]$BuildDir
    )

    if (Test-Path $BuildDir) {
        $cacheFile = Join-Path $BuildDir 'CMakeCache.txt'
        $cmakeFilesDir = Join-Path $BuildDir 'CMakeFiles'
        $makeFile = Join-Path $BuildDir 'Makefile'

        if (Test-Path $cacheFile) { Remove-Item $cacheFile -Force }
        if (Test-Path $makeFile) { Remove-Item $makeFile -Force }
        if (Test-Path $cmakeFilesDir) { Remove-Item $cmakeFilesDir -Recurse -Force }
    }

    & $cmake -S . -B $BuildDir -G 'MinGW Makefiles' `
        "-DCMAKE_CXX_COMPILER=$($compiler)" `
        "-DCMAKE_PREFIX_PATH=$($qtPrefix)"

    if ($LASTEXITCODE -ne 0) {
        throw "Fallo la configuracion en $BuildDir"
    }
}

function Invoke-Build {
    param(
        [string]$BuildDir
    )

    & $cmake --build $BuildDir -j

    if ($LASTEXITCODE -ne 0) {
        throw "Fallo la compilacion en $BuildDir"
    }
}

function Ensure-Build {
    param(
        [string]$PreferredBuildDir
    )

    if (-not (Test-Path $PreferredBuildDir) -or -not (Test-BuildConfigured -BuildDir $PreferredBuildDir)) {
        Invoke-Configure -BuildDir $PreferredBuildDir
    }

    Invoke-Build -BuildDir $PreferredBuildDir
}

function Start-Gui {
    param(
        [string]$BuildDir
    )

    $guiPath = Join-Path $BuildDir $guiExeName

    try {
        & $guiPath
        return
    } catch {
        $message = $_.Exception.Message

        if ($message -match 'Application Control policy has blocked this file' -and $BuildDir -eq $primaryBuildDir) {
            Write-Host 'Windows bloqueo el GUI en build. Probando build_alt...'
            Ensure-Build -PreferredBuildDir $altBuildDir
            try {
                & (Join-Path $altBuildDir $guiExeName)
            } catch {
                $altMessage = $_.Exception.Message
                if ($altMessage -match 'Application Control policy has blocked this file') {
                    throw 'Windows bloquea la ejecucion del GUI. Mueve el proyecto fuera de OneDrive o permite medlang_gui.exe en la politica de seguridad.'
                }
                throw
            }
            return
        }

        throw
    }
}

if ($Mode -eq 'configure') {
    Invoke-Configure -BuildDir $primaryBuildDir
    exit 0
}

if ($Mode -eq 'build') {
    Ensure-Build -PreferredBuildDir $primaryBuildDir
    exit 0
}

if ($Mode -eq 'gui') {
    Ensure-Build -PreferredBuildDir $primaryBuildDir
    Start-Gui -BuildDir $primaryBuildDir
    exit 0
}

if ($Mode -eq 'cli') {
    Ensure-Build -PreferredBuildDir $primaryBuildDir
    $cliPath = Join-Path $primaryBuildDir $cliExeName
    & $cliPath $MedFile
    exit $LASTEXITCODE
}