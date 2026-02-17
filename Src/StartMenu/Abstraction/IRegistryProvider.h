#pragma once

#include <string>

namespace StartMenu {
namespace Abstraction {

class IRegistryProvider {
public:
    virtual ~IRegistryProvider() = default;

    // CONTRACT: registry-drives-config
    // WINE_NOTE: Native registry behavior differs under Wine/ReactOS; test with in-memory provider.
    virtual bool TryReadString(const std::string& key, const std::string& valueName, std::string& outValue) const = 0;
    virtual bool TryReadBool(const std::string& key, const std::string& valueName, bool& outValue) const = 0;
    virtual void WriteString(const std::string& key, const std::string& valueName, const std::string& value) = 0;
    virtual void WriteBool(const std::string& key, const std::string& valueName, bool value) = 0;
};

} // namespace Abstraction
} // namespace StartMenu

// [CODEX] Last modified by: Codex
// [CODEX] Phase: 2
// [CODEX] Summary: Defined platform-neutral registry abstraction interface.
