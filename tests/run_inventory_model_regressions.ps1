param([string]$VisualStudio = 'C:\Program Files\Microsoft Visual Studio\18\Community')
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$buildDirectory = Join-Path $PSScriptRoot 'out\inventory_model'
$vcvars = Join-Path $VisualStudio 'VC\Auxiliary\Build\vcvars64.bat'
$source = Join-Path $PSScriptRoot 'inventory_model_regressions.cpp'
$imgui = Join-Path $root 'manager\external\imgui'
New-Item -ItemType Directory -Path $buildDirectory -Force | Out-Null
$agentMesh = Join-Path $root 'build\agent-assets\seal_team_6.obj'
if (Test-Path -LiteralPath $agentMesh) {
    New-Item -ItemType Directory -Path (Join-Path $buildDirectory 'models') -Force | Out-Null
    Copy-Item -LiteralPath $agentMesh -Destination (Join-Path $buildDirectory 'models\seal_team_6.obj')
}
$command = 'call "{0}" >nul && cl.exe /nologo /std:c++20 /EHsc /W4 /O2 /utf-8 /DWIN32_LEAN_AND_MEAN /DNOMINMAX "{1}" "{2}\imgui.cpp" "{2}\imgui_draw.cpp" "{2}\imgui_tables.cpp" "{2}\imgui_widgets.cpp" /Fe:inventory_model_regressions.exe /link user32.lib' -f $vcvars,$source,$imgui
Push-Location $buildDirectory
try {
    & $env:ComSpec /d /c $command
    if ($LASTEXITCODE -ne 0) { throw "Inventory/model compilation failed: $LASTEXITCODE" }
    & (Join-Path $buildDirectory 'inventory_model_regressions.exe')
    if ($LASTEXITCODE -ne 0) { throw "Inventory/model regressions failed: $LASTEXITCODE" }
} finally { Pop-Location }
