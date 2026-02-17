#include "StubShellIntegration.h"

#include <map>
#include <utility>

namespace StartMenu {
namespace Abstraction {
namespace {

class StubRegistryProvider : public IRegistryProvider {
public:
    bool TryReadString(const std::string& key, const std::string& valueName, std::string& outValue) const override {
        const auto mapKey = key + ":" + valueName;
        const auto it = stringValues.find(mapKey);
        if (it == stringValues.end()) {
            return false;
        }
        outValue = it->second;
        return true;
    }

    bool TryReadBool(const std::string& key, const std::string& valueName, bool& outValue) const override {
        const auto mapKey = key + ":" + valueName;
        const auto it = boolValues.find(mapKey);
        if (it == boolValues.end()) {
            return false;
        }
        outValue = it->second;
        return true;
    }

    void WriteString(const std::string& key, const std::string& valueName, const std::string& value) override {
        stringValues[key + ":" + valueName] = value;
    }

    void WriteBool(const std::string& key, const std::string& valueName, bool value) override {
        boolValues[key + ":" + valueName] = value;
    }

private:
    std::map<std::string, std::string> stringValues;
    std::map<std::string, bool> boolValues;
};

class StubTaskbarAccess : public ITaskbarAccess {
public:
    bool ReplaceStartButton(const OpaqueWindowHandle& taskbarHandle, const std::string& imageId) override {
        lastTaskbar = taskbarHandle;
        lastImageId = imageId;
        replaceCallCount++;
        return taskbarHandle.IsValid() && !imageId.empty();
    }

    OpaqueWindowHandle GetPrimaryTaskbar() const override { return OpaqueWindowHandle{1}; }

    int replaceCallCount{0};
    OpaqueWindowHandle lastTaskbar{};
    std::string lastImageId;
};

class StubHotkeyManager : public IHotkeyManager {
public:
    bool RegisterHotkey(const std::string& id, HotkeyCallback callback) override {
        callbacks[id] = std::move(callback);
        return true;
    }

    void UnregisterHotkey(const std::string& id) override { callbacks.erase(id); }

    bool TriggerHotkey(const std::string& id) override {
        const auto it = callbacks.find(id);
        if (it == callbacks.end()) {
            return false;
        }
        it->second();
        return true;
    }

private:
    std::map<std::string, HotkeyCallback> callbacks;
};

class StubSkinLoader : public ISkinLoader {
public:
    SkinDescriptor LoadSkin(const std::string& requestedSkin) const override {
        const auto it = skins.find(requestedSkin);
        if (it != skins.end()) {
            return it->second;
        }
        return SkinDescriptor{requestedSkin, "", false};
    }

    SkinDescriptor GetDefaultSkin() const override { return defaultSkin; }

    std::map<std::string, SkinDescriptor> skins{{"Default", {"Default", "default.skin", true}}};
    SkinDescriptor defaultSkin{"Default", "default.skin", true};
};

class StubDisplayManager : public IDisplayManager {
public:
    std::vector<MonitorInfo> GetMonitors() const override { return monitors; }

    std::vector<MonitorInfo> monitors{{0, true, 1.0F, {0, 0, 1920, 1080}}};
};

class StubMenuDataProvider : public IMenuDataProvider {
public:
    std::vector<MenuItem> EnumerateRootItems() const override { return rootItems; }

    std::vector<MenuItem> rootItems{{"programs", "Programs", true, {{"calc", "Calculator", false, {}}}}};
};

class StubShellIntegration final : public IShellIntegration {
public:
    explicit StubShellIntegration(PlatformCapabilityFlags capabilitiesIn) : capabilities(capabilitiesIn) {}

    void Initialize() override {
        hotkeys.RegisterHotkey("menu.toggle", [this]() {
            if (menuOpen) {
                CloseMenu();
            } else {
                OpenMenu();
            }
        });

        auto config = LoadConfiguration();
        if (config.replaceStartButton) {
            taskbar.ReplaceStartButton(taskbar.GetPrimaryTaskbar(), "default-start-button");
        }
    }

    void OpenMenu() override { menuOpen = true; }
    void CloseMenu() override { menuOpen = false; }
    bool IsMenuOpen() const override { return menuOpen; }

    StartMenuConfiguration LoadConfiguration() override {
        StartMenuConfiguration cfg;

        std::string skinName;
        if (registry.TryReadString("StartMenu", "Skin", skinName)) {
            cfg.skinName = skinName;
        }

        bool replace = true;
        if (registry.TryReadBool("StartMenu", "ReplaceStartButton", replace)) {
            cfg.replaceStartButton = replace;
        }

        cfg.enableHighRiskFeatures = !capabilities.runningOnWine;
        return cfg;
    }

    std::vector<MenuItem> PopulateMenu() const override { return menuData.EnumerateRootItems(); }

    ITaskbarAccess& Taskbar() override { return taskbar; }
    IRegistryProvider& Registry() override { return registry; }
    IHotkeyManager& Hotkeys() override { return hotkeys; }
    ISkinLoader& Skins() override { return skins; }
    IDisplayManager& Display() override { return display; }
    IMenuDataProvider& MenuData() override { return menuData; }
    PlatformCapabilityFlags Capabilities() const override { return capabilities; }

private:
    PlatformCapabilityFlags capabilities{};
    bool menuOpen{false};
    StubTaskbarAccess taskbar{};
    StubRegistryProvider registry{};
    StubHotkeyManager hotkeys{};
    StubSkinLoader skins{};
    StubDisplayManager display{};
    StubMenuDataProvider menuData{};
};

} // namespace

std::unique_ptr<IShellIntegration> CreateStubShellIntegration(const PlatformCapabilityFlags& capabilities) {
    return std::unique_ptr<IShellIntegration>(new StubShellIntegration(capabilities));
}

} // namespace Abstraction
} // namespace StartMenu

// [CODEX] Last modified by: Codex
// [CODEX] Phase: 2
// [CODEX] Summary: Implemented in-memory stub shell integration with no Win32 calls.
