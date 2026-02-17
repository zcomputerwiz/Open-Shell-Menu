#pragma once

#include "PlatformTypes.h"

namespace StartMenu {
namespace Abstraction {

class ITaskbarAccess {
public:
    virtual ~ITaskbarAccess() = default;

    // CONTRACT: start-button-replacement
    // WINE_NOTE: Shell_TrayWnd/ReBarWindow32 assumptions differ in Wine explorer.
    virtual bool ReplaceStartButton(const OpaqueWindowHandle& taskbarHandle, const std::string& imageId) = 0;
    virtual OpaqueWindowHandle GetPrimaryTaskbar() const = 0;
};

} // namespace Abstraction
} // namespace StartMenu

// [CODEX] Last modified by: Codex
// [CODEX] Phase: 2
// [CODEX] Summary: Added taskbar/start-button abstraction interface.
