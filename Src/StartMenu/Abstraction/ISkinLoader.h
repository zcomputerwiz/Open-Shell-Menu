#pragma once

#include "PlatformTypes.h"

namespace StartMenu {
namespace Abstraction {

class ISkinLoader {
public:
    virtual ~ISkinLoader() = default;

    // CONTRACT: skin-load-fallback
    // WINE_NOTE: Filesystem paths and theming assets can resolve differently in Wine prefixes.
    virtual SkinDescriptor LoadSkin(const std::string& requestedSkin) const = 0;
    virtual SkinDescriptor GetDefaultSkin() const = 0;
};

} // namespace Abstraction
} // namespace StartMenu

// [CODEX] Last modified by: Codex
// [CODEX] Phase: 2
// [CODEX] Summary: Added skin-loading abstraction with fallback support.
