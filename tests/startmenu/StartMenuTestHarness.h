#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../../Src/StartMenu/Abstraction/StubImplementations/StubShellIntegration.h"

namespace StartMenuTests {

using StartMenu::Abstraction::CreateStubShellIntegration;
using StartMenu::Abstraction::IMenuDataProvider;
using StartMenu::Abstraction::IRegistryProvider;
using StartMenu::Abstraction::IShellIntegration;
using StartMenu::Abstraction::MenuItem;
using StartMenu::Abstraction::PlatformCapabilityFlags;
using StartMenu::Abstraction::StartMenuConfiguration;

class MockRegistryProvider final : public IRegistryProvider {
public:
    bool TryReadString(const std::string& key, const std::string& valueName, std::string& outValue) const override {
        const auto it = stringData.find(key + ":" + valueName);
        if (it == stringData.end()) {
            return false;
        }
        outValue = it->second;
        return true;
    }

    bool TryReadBool(const std::string& key, const std::string& valueName, bool& outValue) const override {
        const auto it = boolData.find(key + ":" + valueName);
        if (it == boolData.end()) {
            return false;
        }
        outValue = it->second;
        return true;
    }

    void WriteString(const std::string& key, const std::string& valueName, const std::string& value) override {
        stringData[key + ":" + valueName] = value;
    }

    void WriteBool(const std::string& key, const std::string& valueName, bool value) override {
        boolData[key + ":" + valueName] = value;
    }

private:
    std::map<std::string, std::string> stringData;
    std::map<std::string, bool> boolData;
};

class FakeMenuDataProvider final : public IMenuDataProvider {
public:
    std::vector<MenuItem> EnumerateRootItems() const override { return fixedTree; }

    std::vector<MenuItem> fixedTree{{"root-programs", "Programs", true,
        {{"notepad", "Notepad", false, {}}, {"paint", "Paint", false, {}}}}};
};



class FakeHotkeyManager {
public:
    bool Register(const std::string& id) {
        registrations.push_back(id);
        return true;
    }

    bool WasRegistered(const std::string& id) const {
        for (const auto& value : registrations) {
            if (value == id) {
                return true;
            }
        }
        return false;
    }

private:
    std::vector<std::string> registrations;
};

struct PlatformCapabilityConstants {
    static constexpr bool kDefaultRunningOnWine = false;
    static constexpr bool kDefaultRunningOnReactOS = false;
};

class TestEnvironmentGuard {
public:
    explicit TestEnvironmentGuard(PlatformCapabilityFlags& flagsRef)
        : flags(flagsRef), saved(flagsRef) {}

    ~TestEnvironmentGuard() { flags = saved; }

private:
    PlatformCapabilityFlags& flags;
    PlatformCapabilityFlags saved;
};

class StartMenuTestHarness {
public:
    explicit StartMenuTestHarness(const PlatformCapabilityFlags& flags = {})
        : integration(CreateStubShellIntegration(flags)) {
        integration->Initialize();
    }

    IShellIntegration& Shell() { return *integration; }
    const IShellIntegration& Shell() const { return *integration; }

private:
    std::unique_ptr<IShellIntegration> integration;
};

} // namespace StartMenuTests

// [CODEX] Last modified by: Codex
// [CODEX] Phase: 3
// [CODEX] Summary: Replaced Win32 simulator with abstraction-driven test harness helpers.
