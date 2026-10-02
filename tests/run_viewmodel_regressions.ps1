param([string]$VisualStudio = 'C:\Program Files\Microsoft Visual Studio\18\Community')
$ErrorActionPreference = 'Stop'
$buildDirectory = Join-Path $PSScriptRoot 'out\viewmodel'
$vcvars = Join-Path $VisualStudio 'VC\Auxiliary\Build\vcvars64.bat'
$source = Join-Path $PSScriptRoot 'viewmodel_regressions.cpp'
New-Item -ItemType Directory -Path $buildDirectory -Force | Out-Null
$command = 'call "{0}" >nul && cl.exe /nologo /std:c++20 /EHsc /W4 /O2 /DWIN32_LEAN_AND_MEAN /DNOMINMAX "{1}" /Fe:viewmodel_regressions.exe' -f $vcvars,$source
Push-Location $buildDirectory
try {
    & $env:ComSpec /d /c $command
    if ($LASTEXITCODE -ne 0) { throw "Viewmodel test compilation failed: $LASTEXITCODE" }
    & (Join-Path $buildDirectory 'viewmodel_regressions.exe')
    if ($LASTEXITCODE -ne 0) { throw "Viewmodel regressions failed: $LASTEXITCODE" }
} finally { Pop-Location }
