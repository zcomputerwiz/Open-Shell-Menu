# Build Environment Notes: StartMenu Abstraction Effort

## ATL Usage Inventory

Observed ATL headers in StartMenu scope:
- `atlbase.h`, `atltypes.h`, `atlstr.h`, `atlwin.h` in `Src/StartMenu/stdafx.h` and `Src/StartMenu/StartMenuDLL/stdafx.h`.
- `atlcom.h`, `atlctl.h` and ATL COM registration/message-map macros are used heavily in `StartMenuHelper` COM classes.

Observed ATL classes/macros:
- `CComPtr`, `CComQIPtr`, `CComObject`, `CComObjectRootEx`, `BEGIN_COM_MAP`, `OBJECT_ENTRY_AUTO`, `DECLARE_REGISTRY_RESOURCEID*`.

Portability assessment:
- Core smart pointers/basic ATL COM helpers: **MINGW_COMPATIBLE** only with `mingw-w64-atl` and careful include hygiene.
- COM registration/object-map macros and ATL windowing/message-map code: **MSVC_ONLY** in practical CI terms for this project.

## MFC Usage Inventory

- No direct `afx*.h` includes were found under `Src/StartMenu/` in this audit.
- `CString` usage appears pervasive, but in this tree it is ATL `CString` (`atlstr.h`) rather than MFC class headers.

Portability assessment:
- MFC dependency in StartMenu path: **PORTABLE** (none detected in this slice), but keep full-solution Windows job as guard rail.

## WTL Usage Inventory

- No explicit WTL headers (`atlapp.h`, `atlframe.h`, etc.) were detected in `Src/StartMenu/`.

Portability assessment:
- WTL in StartMenu path: **PORTABLE** (none detected).

## COM Interface Inventory

Non-trivial interfaces observed include:
- Shell COM: `IShellItem`, `IShellFolder`, `IDropTarget`, `IQueryInfo`, `IWebBrowser2`.
- Immersive shell: `IImmersiveShell`, `IImmersiveLauncher80/81/10RS`, `IAppVisibility`, `IImmersiveMonitorService`.
- Helper COM interfaces in `StartMenuHelper` (`IContextMenu`, `IShellExtInit`, task scheduler interfaces).

Portability assessment:
- Standard shell COM interfaces: **SHIM_REQUIRED** for non-Windows test paths.
- Immersive/private COM interfaces: **MSVC_ONLY** and **SHIM_REQUIRED** for architecture (must remain Windows-side).

## `#import` Directives

- No MSVC `#import` directives were detected under `Src/StartMenu/` during this audit.

Portability assessment:
- `#import` usage: **PORTABLE** (none detected).

## MSVC-specific Constructs

Observed:
- `__uuidof(...)` appears in COM-heavy code.
- `#pragma comment(lib, ...)` appears in project sources.

Not observed in this scan:
- `__try`/`__except`, `__assume`, `__declspec(uuid(...))` declarations in StartMenu path.

Portability assessment:
- `__uuidof`: **MINGW_COMPATIBLE** in many cases, but unreliable across mixed ATL/private interfaces.
- `#pragma comment(lib, ...)`: **SHIM_REQUIRED** (convert to build-system linkage in portable targets).

## Abstraction Boundary

### Windows Side (MSVC/Windows only)
- COM object implementations and ATL message-map classes.
- Explorer process injection/hooks and shell class traversal.
- DWM-specific composition behavior and taskbar class manipulation.
- Any helper modules that rely on ATL COM registration/object maps.

### Abstract Interface Layer (portable C++17)
- `IShellIntegration`
- `ITaskbarAccess`
- `IMenuDataProvider`
- `IRegistryProvider`
- `IHotkeyManager`
- `ISkinLoader`
- `IDisplayManager`
- `PlatformTypes` with opaque handles/neutral structs

Constraints enforced in abstraction signatures:
- No `HWND`, `HKEY`, `HRESULT`, COM pointers, or ATL types.
- Only standard C++/STL types plus opaque value structs.

### Platform Implementations
- `WindowsShellIntegration` (future, native Win32/COM/ATL).
- `WineShellIntegration` (future, shim-first reduced behavior).
- `StubShellIntegration` (implemented now, in-memory no-op safe behavior for tests).

### Test Side (Linux/Windows)
- Tests only target abstraction interfaces using `StubShellIntegration`.
- No Win32 API calls or ATL/MFC dependencies in test harness.
- Compiles with `-std=c++17 -Wall -Wextra -Werror` under MinGW-w64.

// [CODEX] Last modified by: Codex
// [CODEX] Phase: 2
// [CODEX] Summary: Documented StartMenu dependency audit and explicit abstraction boundary.
