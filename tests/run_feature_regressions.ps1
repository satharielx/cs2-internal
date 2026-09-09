param([string]$VisualStudio = 'C:\Program Files\Microsoft Visual Studio\18\Community')
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$buildDirectory = Join-Path $PSScriptRoot 'out\features'
$vcvars = Join-Path $VisualStudio 'VC\Auxiliary\Build\vcvars64.bat'
New-Item -ItemType Directory -Path $buildDirectory -Force | Out-Null
$testSource = Join-Path $PSScriptRoot 'feature_regressions.cpp'
$imgui = Join-Path $projectRoot 'manager\external\imgui'
$buildCommand = 'call "{0}" >nul && cl.exe /nologo /std:c++20 /EHsc /W3 /O2 /GL /Gy /Zc:inline /DWIN32_LEAN_AND_MEAN /DNOMINMAX "{1}" "{2}\imgui.cpp" "{2}\imgui_draw.cpp" "{2}\imgui_tables.cpp" "{2}\imgui_widgets.cpp" /Fe:feature_regressions.exe /link /LTCG /OPT:REF user32.lib wininet.lib' -f $vcvars,$testSource,$imgui
Push-Location $buildDirectory
try {
    & $env:ComSpec /d /c $buildCommand
    if ($LASTEXITCODE -ne 0) { throw "Feature test compilation failed: $LASTEXITCODE" }
    & (Join-Path $buildDirectory 'feature_regressions.exe')
    if ($LASTEXITCODE -ne 0) { throw "Feature regressions failed: $LASTEXITCODE" }
} finally { Pop-Location }
