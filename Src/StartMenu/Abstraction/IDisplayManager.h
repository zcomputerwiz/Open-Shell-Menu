#pragma once

#include "PlatformTypes.h"

namespace StartMenu {
namespace Abstraction {

class IDisplayManager {
public:
    virtual ~IDisplayManager() = default;

    // CONTRACT: multi-monitor-fallback
    // WINE_NOTE: Monitor topology and DPI reporting may diverge from Windows APIs in Wine/ReactOS.
    virtual std::vector<MonitorInfo> GetMonitors() const = 0;
};

} // namespace Abstraction
} // namespace StartMenu

// [CODEX] Last modified by: Codex
// [CODEX] Phase: 2
// [CODEX] Summary: Added display/monitor abstraction interface.
