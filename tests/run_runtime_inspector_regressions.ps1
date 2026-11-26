param([string]$VisualStudio = 'C:\Program Files\Microsoft Visual Studio\18\Community')
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$buildDirectory = Join-Path $PSScriptRoot 'out\runtime_inspector'
$vcvars = Join-Path $VisualStudio 'VC\Auxiliary\Build\vcvars64.bat'
$source = Join-Path $PSScriptRoot 'runtime_inspector_regressions.cpp'
$inspector = Join-Path $projectRoot 'manager\core\runtime_inspector.cpp'
$imgui = Join-Path $projectRoot 'manager\external\imgui'
New-Item -ItemType Directory -Path $buildDirectory -Force | Out-Null
$command = 'call "{0}" >nul && cl.exe /nologo /std:c++20 /EHsc /W4 /O2 /utf-8 /DWIN32_LEAN_AND_MEAN /DNOMINMAX "{1}" "{2}" "{3}\imgui.cpp" "{3}\imgui_draw.cpp" "{3}\imgui_tables.cpp" "{3}\imgui_widgets.cpp" /Fe:runtime_inspector_regressions.exe /link user32.lib' -f $vcvars,$source,$inspector,$imgui
Push-Location $buildDirectory
try {
    & $env:ComSpec /d /c $command
    if ($LASTEXITCODE -ne 0) { throw "Inspector test compilation failed: $LASTEXITCODE" }
    & (Join-Path $buildDirectory 'runtime_inspector_regressions.exe')
    if ($LASTEXITCODE -ne 0) { throw "Inspector regressions failed: $LASTEXITCODE" }
} finally { Pop-Location }
