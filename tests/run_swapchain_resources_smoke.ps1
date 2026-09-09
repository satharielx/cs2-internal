param(
    [string]$VisualStudio = 'C:\Program Files\Microsoft Visual Studio\18\Community'
)

$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$buildDirectory = Join-Path $PSScriptRoot 'out'
$vcvars = Join-Path $VisualStudio 'VC\Auxiliary\Build\vcvars64.bat'
if (-not (Test-Path -LiteralPath $vcvars)) {
    throw "MSVC x64 environment script not found: $vcvars"
}
New-Item -ItemType Directory -Path $buildDirectory -Force | Out-Null

# Keep compiler output under tests/out (already ignored by the repository).
$testSource = Join-Path $PSScriptRoot 'swapchain_resources_smoke.cpp'
$actualSource = Join-Path $projectRoot 'manager\core\interfaces.cpp'
$buildCommand = 'call "{0}" >nul && cl.exe /nologo /std:c++20 /EHsc /W4 /utf-8 /DWIN32_LEAN_AND_MEAN /DNOMINMAX "{1}" "{2}" /Fe:swapchain_resources_smoke.exe /link d3d11.lib dxgi.lib user32.lib' -f $vcvars, $testSource, $actualSource

Push-Location $buildDirectory
try {
    & $env:ComSpec /d /c $buildCommand
    if ($LASTEXITCODE -ne 0) { throw "Smoke-test compilation failed with exit code $LASTEXITCODE" }
    & (Join-Path $buildDirectory 'swapchain_resources_smoke.exe')
    if ($LASTEXITCODE -ne 0) { throw "Smoke test failed with exit code $LASTEXITCODE" }
}
finally {
    Pop-Location
}
