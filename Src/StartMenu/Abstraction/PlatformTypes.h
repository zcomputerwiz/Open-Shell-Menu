#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace StartMenu {
namespace Abstraction {

struct OpaqueWindowHandle {
    std::uintptr_t value{0};

    bool IsValid() const noexcept { return value != 0; }
};

struct MonitorRect {
    int left{0};
    int top{0};
    int right{0};
    int bottom{0};
};

struct MonitorInfo {
    int id{0};
    bool primary{false};
    float dpiScale{1.0F};
    MonitorRect bounds{};
};

struct SkinDescriptor {
    std::string name;
    std::string path;
    bool found{false};
};

struct MenuItem {
    std::string id;
    std::string displayName;
    bool isFolder{false};
    std::vector<MenuItem> children;
};

struct StartMenuConfiguration {
    std::string skinName{"Default"};
    bool replaceStartButton{true};
    bool enableHighRiskFeatures{true};
};

} // namespace Abstraction
} // namespace StartMenu

// [CODEX] Last modified by: Codex
// [CODEX] Phase: 2
// [CODEX] Summary: Added platform-neutral data types used by abstraction interfaces.
