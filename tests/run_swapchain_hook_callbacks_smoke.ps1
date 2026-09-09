param(
    [string]$VisualStudio = 'C:\Program Files\Microsoft Visual Studio\18\Community'
)

$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$buildDirectory = Join-Path $PSScriptRoot 'out\hook_callbacks'
$vcvars = Join-Path $VisualStudio 'VC\Auxiliary\Build\vcvars64.bat'
if (-not (Test-Path -LiteralPath $vcvars)) {
    throw "MSVC x64 environment script not found: $vcvars"
}
New-Item -ItemType Directory -Path $buildDirectory -Force | Out-Null

$testSource = Join-Path $PSScriptRoot 'swapchain_hook_callbacks_smoke.cpp'
$actualResources = Join-Path $projectRoot 'manager\core\interfaces.cpp'
$buildCommand = 'call "{0}" >nul && cl.exe /nologo /std:c++20 /EHsc /W4 /utf-8 /O2 /GL /Gy /Zc:inline /DWIN32_LEAN_AND_MEAN /DNOMINMAX "{1}" "{2}" /Fe:swapchain_hook_callbacks_smoke.exe /link /LTCG /OPT:REF d3d11.lib dxgi.lib user32.lib' -f $vcvars, $testSource, $actualResources

Push-Location $buildDirectory
try {
    & $env:ComSpec /d /c $buildCommand
    if ($LASTEXITCODE -ne 0) { throw "Callback smoke-test compilation failed with exit code $LASTEXITCODE" }
    & (Join-Path $buildDirectory 'swapchain_hook_callbacks_smoke.exe')
    if ($LASTEXITCODE -ne 0) { throw "Callback smoke test failed with exit code $LASTEXITCODE" }
}
finally {
    Pop-Location
}
