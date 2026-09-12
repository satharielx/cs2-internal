param([string]$VisualStudio = 'C:\Program Files\Microsoft Visual Studio\18\Community')
$ErrorActionPreference = 'Stop'
$buildDirectory = Join-Path $PSScriptRoot 'out\loadout'
$vcvars = Join-Path $VisualStudio 'VC\Auxiliary\Build\vcvars64.bat'
New-Item -ItemType Directory -Path $buildDirectory -Force | Out-Null
$testSource = Join-Path $PSScriptRoot 'loadout_import.cpp'
$buildCommand = 'call "{0}" >nul && cl.exe /nologo /std:c++20 /EHsc /DWIN32_LEAN_AND_MEAN /DNOMINMAX "{1}" /Fe:loadout_import.exe' -f $vcvars,$testSource
Push-Location $buildDirectory
try {
    & $env:ComSpec /d /c $buildCommand
    if ($LASTEXITCODE -ne 0) { throw 'Loadout test compilation failed' }
    & (Join-Path $buildDirectory 'loadout_import.exe')
    if ($LASTEXITCODE -ne 0) { throw 'Loadout import tests failed' }
} finally { Pop-Location }
