# Required Production Source Changes (Not Applied)

The following production-side refactors are required to fully bind tests to real StartMenu behavior without modifying existing non-test source in this change set.

1. Introduce dependency injection seam in StartMenu initialization.
   - Rationale: current `StartMenuDLL` startup path creates/uses global Win32 hooks directly.
   - Needed change: create a factory entry point that accepts an `IShellIntegration` implementation.

2. Route registry reads/writes through `IRegistryProvider`.
   - Rationale: existing settings code reads Win32 registry APIs directly.
   - Needed change: replace direct calls in menu/config code with provider methods.

3. Route taskbar and start-button behavior through `ITaskbarAccess`.
   - Rationale: `Shell_TrayWnd` class walking and `SetWindowSubclass` calls are hardcoded.
   - Needed change: isolate taskbar window discovery and button replacement into Windows implementation.

4. Route hotkey registration and trigger handling through `IHotkeyManager`.
   - Rationale: direct `RegisterHotKey`/hook usage blocks Linux-side tests.
   - Needed change: keep OS calls on Windows implementation side and use abstraction in menu logic.

5. Isolate DWM visual behavior through a feature-gated interface call.
   - Rationale: DWM APIs are platform-dependent and likely no-op/stubbed in non-Windows runtimes.
   - Needed change: create capability check and fallback render path when unsupported.

// [CODEX] Last modified by: Codex
// [CODEX] Phase: 3
// [CODEX] Summary: Listed non-applied production refactors needed to bind abstraction to real code.
