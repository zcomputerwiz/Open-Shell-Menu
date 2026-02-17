#pragma once

#include <functional>
#include <string>

namespace StartMenu {
namespace Abstraction {

class IHotkeyManager {
public:
    virtual ~IHotkeyManager() = default;

    using HotkeyCallback = std::function<void()>;

    // CONTRACT: menu-open-on-hotkey
    // WINE_NOTE: RegisterHotKey and low-level hooks vary across Wine versions.
    virtual bool RegisterHotkey(const std::string& id, HotkeyCallback callback) = 0;
    virtual void UnregisterHotkey(const std::string& id) = 0;
    virtual bool TriggerHotkey(const std::string& id) = 0;
};

} // namespace Abstraction
} // namespace StartMenu

// [CODEX] Last modified by: Codex
// [CODEX] Phase: 2
// [CODEX] Summary: Added hotkey abstraction for platform-neutral menu toggle behavior.
