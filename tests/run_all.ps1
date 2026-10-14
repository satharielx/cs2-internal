param([string]$VisualStudio = 'C:\Program Files\Microsoft Visual Studio\18\Community')
$ErrorActionPreference = 'Stop'
$suites = @('console_regressions','config_regressions','feature_regressions','grenade_regressions','rcs_regressions','lighting_regressions','viewmodel_regressions','skybox_regressions','skin_hud_regressions','menu_mouse_regressions','game_state_regressions','loadout_import','toggle_regressions','pattern_scanner_smoke','swapchain_resources_smoke','swapchain_hook_callbacks_smoke')
foreach ($suite in $suites) {
    Write-Host "Running $suite"
    & (Join-Path $PSScriptRoot "run_$suite.ps1") -VisualStudio $VisualStudio
}
Write-Host 'All regression suites passed.'
