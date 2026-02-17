# StartMenu Contract Coverage

| Contract ID | Contract Description | Test Function | File | Wine Risk | Status |
|---|---|---|---|---|---|
| menu-open-on-hotkey | Hotkey toggles menu open from closed state | TestMenuOpenOnHotkey | tests/startmenu/StartMenuBehaviorTests.cpp | LOW | COVERED |
| registry-drives-config | Registry values flow into menu configuration | TestRegistrySettingsAppliedToMenuConfig | tests/startmenu/StartMenuBehaviorTests.cpp | LOW | COVERED |
| start-button-replacement | Init path triggers start button replacement behavior | TestStartButtonReplacementInitializationPath | tests/startmenu/StartMenuBehaviorTests.cpp | MEDIUM | COVERED |
| skin-load-fallback | Missing skin falls back to default without crash | TestMissingSkinFileGracefulFallback | tests/startmenu/StartMenuBehaviorTests.cpp | LOW | COVERED |
| multi-monitor-fallback | Single-monitor fallback path remains valid | TestMultiMonitorFallbackSingleDisplay | tests/startmenu/StartMenuBehaviorTests.cpp | MEDIUM | COVERED |
| menu-population | Shell namespace data maps to expected menu tree | TestShellNamespaceEnumerationPopulation | tests/startmenu/StartMenuBehaviorTests.cpp | MEDIUM | COVERED |
| wine-feature-degradation | Wine mode disables high-risk behavior path | TestWineCapabilityFlagReducedFeatureSet | tests/startmenu/StartMenuBehaviorTests.cpp | HIGH | COVERED |

// [CODEX] Last modified by: Codex
// [CODEX] Phase: 3
// [CODEX] Summary: Added contract-to-test mapping table for abstraction behavior tests.
