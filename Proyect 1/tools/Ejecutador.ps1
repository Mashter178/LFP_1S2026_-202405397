param(
    [ValidateSet('configure','build','gui','cli')]
    [string]$Mode = 'gui'
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
$cliExeName = 'medlang_cli.exe'
$guiExeName = 'medlang_gui.exe'
$sampleMed = Join-Path $projectRoot 'test/hospital_valido_01.med'

$env:PATH = "$qtBin;$env:PATH"
$env:QT_PLUGIN_PATH = $qtPlugins
$env:QT_QPA_PLATFORM_PLUGIN_PATH = $qtPlatformPlugins

function Test-MedLangFileLocked {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Path
    )

    if (-not (Test-Path $Path)) {
        return $false
    }

    try {
        $stream = [System.IO.File]::Open($Path, 'Open', 'ReadWrite', 'None')
        $stream.Close()
        return $false
    } catch {
        return $true
    }
}

function Invoke-MedLangConfigure {
    param(
        [Parameter(Mandatory = $true)]
        [string]$BuildDir
    )

    try {
        $output = & $cmake `
            -S $projectRoot `
            -B $BuildDir `
            -G 'MinGW Makefiles' `
            -DCMAKE_CXX_COMPILER=$compiler `
            -DCMAKE_PREFIX_PATH=$qtPrefix 2>&1
    } catch {
        $output = $_.Exception.Message
    }

    if ($output) {
        $output | ForEach-Object { Write-Host $_ }
    }

    if ($LASTEXITCODE -ne 0) {
        throw "La configuracion fallo en $BuildDir."
    }
}

function Invoke-MedLangBuild {
    param(
        [Parameter(Mandatory = $true)]
        [string]$BuildDir
    )

    try {
        $output = & $cmake --build $BuildDir -j 2>&1
    } catch {
        $output = $_.Exception.Message
    }

    if ($output) {
        $output | ForEach-Object { Write-Host $_ }
    }

    return [pscustomobject]@{
        Success = ($LASTEXITCODE -eq 0)
        OutputText = ($output | Out-String)
    }
}

function Invoke-MedLangCompile {
    param(
        [ValidateSet('build','build_alt')]
        [string]$PreferredBuildDirName = 'build',
        [switch]$Clean,
        [switch]$Reconfigure,
        [switch]$AllowFallbackToAlt = $true
    )

    $buildDirNames = @($PreferredBuildDirName)
    if ($AllowFallbackToAlt -and $PreferredBuildDirName -eq 'build') {
        $buildDirNames += 'build_alt'
    }

    foreach ($buildDirName in $buildDirNames) {
        $buildDir = Join-Path $projectRoot $buildDirName

        if ($Clean -and (Test-Path $buildDir)) {
            Remove-Item $buildDir -Recurse -Force
        }

        if ($Reconfigure -or -not (Test-Path $buildDir)) {
            Invoke-MedLangConfigure -BuildDir $buildDir
        }

        $buildResult = Invoke-MedLangBuild -BuildDir $buildDir
        if ($buildResult.Success) {
            return $buildDir
        }

        if ($buildDirName -eq 'build' -and $AllowFallbackToAlt -and $buildResult.OutputText -match 'Permission denied|cannot open output file medlang_gui\.exe') {
            Write-Host 'El ejecutable principal esta bloqueado. Reintentando en build_alt...'
            continue
        }

        throw "La compilacion fallo en $buildDir."
    }

    throw 'No se pudo compilar MedLang.'
}

function Get-UsableBuildDir {
    param(
        [Parameter(Mandatory = $true)]
        [string]$ExecutableName
    )

    $primaryExe = Join-Path $primaryBuildDir $ExecutableName
    if ((Test-Path $primaryExe) -and -not (Test-MedLangFileLocked -Path $primaryExe)) {
        return $primaryBuildDir
    }

    $altExe = Join-Path $altBuildDir $ExecutableName
    if ((Test-Path $altExe) -and -not (Test-MedLangFileLocked -Path $altExe)) {
        return $altBuildDir
    }

    return $null
}

function Ensure-BuildDir {
    param(
        [Parameter(Mandatory = $true)]
        [string]$ExecutableName
    )

    $usableBuildDir = Get-UsableBuildDir -ExecutableName $ExecutableName
    if ($usableBuildDir) {
        return $usableBuildDir
    }

    $preferredBuildDirName = 'build'
    $primaryExe = Join-Path $primaryBuildDir $ExecutableName
    if ((Test-Path $primaryExe) -and (Test-MedLangFileLocked -Path $primaryExe)) {
        $preferredBuildDirName = 'build_alt'
    }

    return Invoke-MedLangCompile -PreferredBuildDirName $preferredBuildDirName
}

Set-Location $projectRoot

switch ($Mode) {
    'configure' {
        Invoke-MedLangConfigure -BuildDir $primaryBuildDir
    }
    'build' {
        Invoke-MedLangCompile | Out-Null
    }
    'gui' {
        Get-Process medlang_gui -ErrorAction SilentlyContinue | Stop-Process -Force
        $preferredBuildDirName = 'build'
        $primaryExe = Join-Path $primaryBuildDir $guiExeName
        if ((Test-Path $primaryExe) -and (Test-MedLangFileLocked -Path $primaryExe)) {
            $preferredBuildDirName = 'build_alt'
        }
        $buildDir = Invoke-MedLangCompile -PreferredBuildDirName $preferredBuildDirName
        & (Join-Path $buildDir $guiExeName)
    }
    'cli' {
        $buildDir = Invoke-MedLangCompile
        & (Join-Path $buildDir $cliExeName) $sampleMed
    }
}