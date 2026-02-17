#pragma once

#include "IDisplayManager.h"
#include "IHotkeyManager.h"
#include "IMenuDataProvider.h"
#include "IRegistryProvider.h"
#include "ISkinLoader.h"
#include "ITaskbarAccess.h"

namespace StartMenu {
namespace Abstraction {

struct PlatformCapabilityFlags {
    bool runningOnWine{false};
    bool runningOnReactOS{false};
    bool supportsDwmComposition{true};
    bool supportsExplorerInjection{true};
};

class IShellIntegration {
public:
    virtual ~IShellIntegration() = default;

    // CONTRACT: menu-open-on-hotkey
    // WINE_NOTE: Must degrade gracefully when shell hooks are unavailable.
    virtual void Initialize() = 0;
    virtual void OpenMenu() = 0;
    virtual void CloseMenu() = 0;
    virtual bool IsMenuOpen() const = 0;

    // CONTRACT: registry-drives-config
    virtual StartMenuConfiguration LoadConfiguration() = 0;

    // CONTRACT: menu-population
    virtual std::vector<MenuItem> PopulateMenu() const = 0;

    virtual ITaskbarAccess& Taskbar() = 0;
    virtual IRegistryProvider& Registry() = 0;
    virtual IHotkeyManager& Hotkeys() = 0;
    virtual ISkinLoader& Skins() = 0;
    virtual IDisplayManager& Display() = 0;
    virtual IMenuDataProvider& MenuData() = 0;
    virtual PlatformCapabilityFlags Capabilities() const = 0;
};

} // namespace Abstraction
} // namespace StartMenu

// [CODEX] Last modified by: Codex
// [CODEX] Phase: 2
// [CODEX] Summary: Defined root shell integration abstraction and capability flags.
