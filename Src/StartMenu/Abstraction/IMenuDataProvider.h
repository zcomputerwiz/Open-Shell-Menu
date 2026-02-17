#pragma once

#include "PlatformTypes.h"

namespace StartMenu {
namespace Abstraction {

class IMenuDataProvider {
public:
    virtual ~IMenuDataProvider() = default;

    // CONTRACT: menu-population
    // WINE_NOTE: Shell namespace COM traversal may be partial in Wine/ReactOS.
    virtual std::vector<MenuItem> EnumerateRootItems() const = 0;
};

} // namespace Abstraction
} // namespace StartMenu

// [CODEX] Last modified by: Codex
// [CODEX] Phase: 2
// [CODEX] Summary: Added menu data provider abstraction for shell namespace enumeration.
