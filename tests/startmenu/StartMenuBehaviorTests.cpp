#include "StartMenuTestHarness.h"

#include <functional>
#include <iostream>
#include <string>
#include <vector>

using StartMenuTests::StartMenuTestHarness;
using StartMenuTests::TestEnvironmentGuard;
using StartMenu::Abstraction::PlatformCapabilityFlags;

#define TEST_ASSERT(expr)                                                                                               \
    do {                                                                                                                \
        if (!(expr)) {                                                                                                  \
            std::cerr << "Assertion failed: " #expr " at " << __FILE__ << ":" << __LINE__ << std::endl;             \
            return false;                                                                                               \
        }                                                                                                               \
    } while (false)

// CONTRACT: menu-open-on-hotkey
// WINE_RISK: LOW
bool TestMenuOpenOnHotkey() {
    PlatformCapabilityFlags flags{};
    TestEnvironmentGuard guard(flags);
    StartMenuTestHarness harness(flags);

    TEST_ASSERT(!harness.Shell().IsMenuOpen());
    TEST_ASSERT(harness.Shell().Hotkeys().TriggerHotkey("menu.toggle"));
    TEST_ASSERT(harness.Shell().IsMenuOpen());
    return true;
}

// CONTRACT: registry-drives-config
// WINE_RISK: LOW
bool TestRegistrySettingsAppliedToMenuConfig() {
    PlatformCapabilityFlags flags{};
    TestEnvironmentGuard guard(flags);
    StartMenuTestHarness harness(flags);

    harness.Shell().Registry().WriteString("StartMenu", "Skin", "ClassicTest");
    harness.Shell().Registry().WriteBool("StartMenu", "ReplaceStartButton", false);

    const auto cfg = harness.Shell().LoadConfiguration();
    TEST_ASSERT(cfg.skinName == "ClassicTest");
    TEST_ASSERT(!cfg.replaceStartButton);
    return true;
}

// CONTRACT: start-button-replacement
// WINE_RISK: MEDIUM
bool TestStartButtonReplacementInitializationPath() {
    PlatformCapabilityFlags flags{};
    TestEnvironmentGuard guard(flags);
    StartMenuTestHarness harness(flags);

    const auto taskbarHandle = harness.Shell().Taskbar().GetPrimaryTaskbar();
    TEST_ASSERT(taskbarHandle.IsValid());
    TEST_ASSERT(harness.Shell().Taskbar().ReplaceStartButton(taskbarHandle, "test-image"));
    return true;
}

// CONTRACT: skin-load-fallback
// WINE_RISK: LOW
bool TestMissingSkinFileGracefulFallback() {
    PlatformCapabilityFlags flags{};
    TestEnvironmentGuard guard(flags);
    StartMenuTestHarness harness(flags);

    const auto missing = harness.Shell().Skins().LoadSkin("MissingSkin");
    TEST_ASSERT(!missing.found);
    const auto fallback = harness.Shell().Skins().GetDefaultSkin();
    TEST_ASSERT(fallback.found);
    TEST_ASSERT(fallback.name == "Default");
    return true;
}

// CONTRACT: multi-monitor-fallback
// WINE_RISK: MEDIUM
bool TestMultiMonitorFallbackSingleDisplay() {
    PlatformCapabilityFlags flags{};
    TestEnvironmentGuard guard(flags);
    StartMenuTestHarness harness(flags);

    const auto monitors = harness.Shell().Display().GetMonitors();
    TEST_ASSERT(monitors.size() == 1);
    TEST_ASSERT(monitors.front().primary);
    return true;
}

// CONTRACT: menu-population
// WINE_RISK: MEDIUM
bool TestShellNamespaceEnumerationPopulation() {
    PlatformCapabilityFlags flags{};
    TestEnvironmentGuard guard(flags);
    StartMenuTestHarness harness(flags);

    const auto items = harness.Shell().PopulateMenu();
    TEST_ASSERT(items.size() == 1);
    TEST_ASSERT(items.front().displayName == "Programs");
    TEST_ASSERT(items.front().children.size() == 1);
    TEST_ASSERT(items.front().children.front().displayName == "Calculator");
    return true;
}

// CONTRACT: wine-feature-degradation
// WINE_RISK: HIGH
bool TestWineCapabilityFlagReducedFeatureSet() {
    PlatformCapabilityFlags flags{};
    flags.runningOnWine = true;
    TestEnvironmentGuard guard(flags);
    StartMenuTestHarness harness(flags);

    const auto cfg = harness.Shell().LoadConfiguration();
    TEST_ASSERT(!cfg.enableHighRiskFeatures);
    return true;
}

int main() {
    const std::vector<std::pair<std::string, std::function<bool()>>> tests = {
        {"TestMenuOpenOnHotkey", TestMenuOpenOnHotkey},
        {"TestRegistrySettingsAppliedToMenuConfig", TestRegistrySettingsAppliedToMenuConfig},
        {"TestStartButtonReplacementInitializationPath", TestStartButtonReplacementInitializationPath},
        {"TestMissingSkinFileGracefulFallback", TestMissingSkinFileGracefulFallback},
        {"TestMultiMonitorFallbackSingleDisplay", TestMultiMonitorFallbackSingleDisplay},
        {"TestShellNamespaceEnumerationPopulation", TestShellNamespaceEnumerationPopulation},
        {"TestWineCapabilityFlagReducedFeatureSet", TestWineCapabilityFlagReducedFeatureSet},
    };

    for (const auto& test : tests) {
        if (!test.second()) {
            std::cerr << test.first << " failed" << std::endl;
            return 1;
        }
        std::cout << test.first << " passed" << std::endl;
    }

    std::cout << "All StartMenu abstraction tests passed." << std::endl;
    return 0;
}

// [CODEX] Last modified by: Codex
// [CODEX] Phase: 3
// [CODEX] Summary: Replaced reference-model tests with abstraction-layer contract tests.
