#pragma once

#include <memory>

#include "../IShellIntegration.h"

namespace StartMenu {
namespace Abstraction {

std::unique_ptr<IShellIntegration> CreateStubShellIntegration(const PlatformCapabilityFlags& capabilities = {});

} // namespace Abstraction
} // namespace StartMenu

// [CODEX] Last modified by: Codex
// [CODEX] Phase: 2
// [CODEX] Summary: Declared factory for pure C++ stub shell integration.
