# Shell Integration Inventory: Open-Shell-Menu

## 1. Executive Summary
Open-Shell-Menu (formerly Classic Shell) is deeply integrated with the Windows Shell (Explorer) to provide its Start Menu and Explorer enhancements. This integration is achieved through a combination of well-documented APIs and highly invasive techniques.

### Core Integration Strategies:
- **Process Injection & Hooking**: The Start Menu component (`StartMenu.exe`) injects `StartMenuDLL.dll` into the `explorer.exe` process using `SetWindowsHookEx(WH_GETMESSAGE)`.
- **Window Subclassing**: Both the Start Menu and Classic Explorer components heavily use window subclassing to intercept and modify the behavior of standard shell windows (e.g., Taskbar, Explorer TreeView, Breadcrumbs).
- **IAT Hooking**: In the Start Menu DLL, manual Import Address Table (IAT) patching is performed within the `explorer.exe` process to override internal drawing and animation functions.
- **COM Integration**: Open-Shell implements several COM interfaces (BHO, Context Menu Handlers) and consumes many internal Shell COM interfaces, including some related to the "Immersive Shell" (Win8+).
- **MSAA (Accessibility)**: The "Classic Copy" feature uses Microsoft Active Accessibility to scrape data from and interact with the original Shell file operation dialogs.

### High-Risk Dependencies:
- **IAT Patching**: Fragile and highly dependent on specific DLL export names and ordinals (e.g., `dwmapi.dll` ordinals 138, 140, 141).
- **Hardcoded Window Classes**: Deeply coupled with Explorer's internal window hierarchy (`Shell_TrayWnd`, `WorkerW`, `ReBarWindow32`, etc.).
- **Shell32 Resource IDs**: Relies on specific resource IDs in `shell32.dll` for string comparisons to identify dialogs.
- **Undocumented COM Interfaces**: Uses many undocumented or semi-documented interfaces for Win8/10 integration (`IImmersiveShell`, `IAppVisibility`, etc.).

### Abstraction Candidates:
- Windowing and subclassing (to support Wine/ReactOS differences).
- Hook registration and management.
- Taskbar and Start button interaction logic.
- Explorer BHO and UI customization points.
- IAT hooking (needs a more portable approach or platform-specific implementations).

---

## 2. Win32 API Inventory

| API Name | File(s) | Purpose | Explorer-Specific | Criticality | Notes |
|----------|---------|---------|-------------------|-------------|-------|
| `SetWindowsHookEx` | `StartMenu.cpp`, `StartMenuDLL.cpp`, `ExplorerBHO.cpp`, `ClassicCopy.cpp` | Injecting DLLs and intercepting system-wide or thread-specific messages/events. | No | Critical | Used for WH_GETMESSAGE, WH_CBT, WH_KEYBOARD, WH_MOUSE. |
| `SetWindowSubclass` | `StartMenuDLL.cpp`, `ExplorerBHO.cpp`, `ClassicCopy.cpp` | Intercepting messages for specific shell windows. | No | Critical | Core mechanism for UI customization. |
| `FindWindow` / `FindWindowEx` | `StartMenu.cpp`, `StartMenuDLL.cpp`, `ExplorerBHO.cpp` | Locating Explorer windows like the Taskbar (`Shell_TrayWnd`) or Desktop (`Progman`). | Yes | Critical | Relies on hardcoded class names. |
| `GetWindowThreadProcessId` | `StartMenu.cpp`, `StartMenuDLL.cpp` | Identifying the thread/process of shell windows for hooking. | Yes | Critical | Used to target `explorer.exe`. |
| `RegisterWindowMessage` | `StartMenu.cpp`, `StartMenuDLL.cpp`, `ExplorerBHO.cpp` | Creating custom messages for IPC between `StartMenu.exe` and `StartMenuDLL.dll`. | No | Critical | E.g., `OpenShellMenu.StartMenuMsg`. |
| `PostMessage` / `SendMessage` | Throughout | Communicating with shell windows and internal components. | No | Critical | Extensive use of `WM_SYSCOMMAND` with `SC_TASKLIST`. |
| `ChangeWindowMessageFilterEx` | `StartMenu.cpp` | Allowing specific messages through UAC filter. | No | Critical | Necessary for `TaskbarCreated` and custom messages. |
| `AllowSetForegroundWindow` | `StartMenu.cpp` | Enabling the Start Menu to take focus. | No | Critical | Explorer-specific interaction. |
| `RegisterHotKey` / `UnregisterHotKey` | `StartMenuDLL.cpp` | Intercepting the Windows key and custom shortcuts. | No | Critical | Prevents default Win-key behavior. |
| `GetGUIThreadInfo` | `StartMenuDLL.cpp`, `ExplorerBHO.cpp` | Detecting if a menu or caret is active. | No | Optional | Used for intelligent behavior (e.g., not showing menu when typing). |
| `CreateMutex` | `StartMenu.cpp` | Ensuring single instance per desktop. | No | Critical | Mutex name includes username and desktop name. |
| `BeginBufferedPaint` / `EndBufferedPaint` | `ExplorerBHO.cpp` | Custom drawing of Explorer controls. | No | Optional | Used for Breadcrumbs customization. |
| `SetWindowCompositionAttribute` | `StartMenuDLL.cpp` | Setting taskbar transparency/blur (undocumented). | Yes | Optional | Win10+ specific, used for taskbar skinning. |

---

## 3. Shell API Usage

| API Name | File(s) | Purpose | Explorer-Specific | Criticality | Notes |
|----------|---------|---------|-------------------|-------------|-------|
| `SHAppBarMessage` | `StartMenuDLL.cpp` | Querying taskbar position (`ABM_GETTASKBARPOS`) and state (`ABM_GETSTATE`). | Yes | Critical | Essential for positioning the Start Menu. |
| `SHGetFileInfo` | `ExplorerBHO.cpp`, `StartMenuDLL.cpp` | Retrieving icons and display names for files/folders. | Yes | Critical | Core shell functionality. |
| `SHGetDesktopFolder` | `ExplorerBHO.cpp` | Accessing the root of the shell namespace. | Yes | Critical | Foundation for shell navigation. |
| `SHGetKnownFolderIDList` | `ExplorerBHO.cpp`, `SaveLogFile.cpp` | Locating standard folders (Computer, Programs, etc.). | Yes | Critical | Used for population of menus and navigation. |
| `SHBindToParent` | `ExplorerBHO.cpp` | Navigating the shell namespace hierarchy. | Yes | Critical | |
| `IL*` (PIDL functions) | Throughout | Manipulating shell item identifiers (PIDLs). | Yes | Critical | E.g., `ILCombine`, `ILClone`, `ILFree`, `ILIsEqual`. |
| `ShellExecute` / `ShellExecuteEx` | Throughout | Launching files, folders, or system commands. | Yes | Critical | |
| `SHRestricted` | `StartMenuDLL.cpp` | Respecting Group Policy restrictions. | Yes | Optional | |
| `Path*` (Path functions) | Throughout | String manipulation for file paths. | No | Critical | |

---

## 4. COM Interfaces and Shell Objects

| Interface Name | Creation Method | Expected Behavior | Viability of Stub | Criticality |
|----------------|-----------------|-------------------|-------------------|-------------|
| `IShellBrowser` | `QueryService` (SID_SShellBrowser) | Controlling the Explorer window. | Hard | Critical |
| `IShellView` | `QueryActiveShellView` | Interacting with the current folder view. | Hard | Critical |
| `IFolderView` / `IFolderView2` | `QueryInterface` | Querying items in the view. | Hard | Critical |
| `IWebBrowser2` | `QueryService` (SID_SWebBrowserApp) | Explorer's underlying browser object. | Hard | Critical |
| `IShellFolder` / `IShellFolder2` | `BindToObject` / `SHGetDesktopFolder` | Shell namespace traversal. | Hard | Critical |
| `IAppVisibility` | `CoCreateInstance` (CLSID_MetroMode) | Detecting Win8+ Start Screen state. | Moderate | Critical (Win8+) |
| `IImmersiveShell` | `CoCreateInstance` (CLSID_ImmersiveShell) | Accessing Win8+ shell services. | Hard | Critical (Win8+) |
| `IImmersiveLauncher` | `QueryService` | Controlling the Win8+ Start Screen. | Hard | Critical (Win8+) |
| `IDropTarget` | Implemented/Subclassed | Handling drag and drop on the Start button. | Easy | Critical |
| `IAccessible` | `AccessibleObjectFromWindow` | Scaping data from shell dialogs (MSAA). | Moderate | Critical (Classic Copy) |

---

## 5. Window Class and Message Assumptions

| Class Name | Component | Assumption | Criticality |
|------------|-----------|------------|-------------|
| `Shell_TrayWnd` | Start Menu | The primary taskbar window. | Critical |
| `Shell_SecondaryTrayWnd` | Start Menu | Taskbars on secondary monitors (Win8+). | Critical |
| `Progman` | Start Menu | The desktop window (owner of the shell process). | Critical |
| `WorkerW` | Start Menu | Often a parent of the taskbar or desktop. | Critical |
| `ReBarWindow32` | Explorer / Start Menu | The container for Explorer bands and Taskbar controls. | Critical |
| `MSTaskListWClass` | Start Menu | The list of running applications on the taskbar. | Critical |
| `CabinetWClass` | Explorer | The main Explorer window class. | Critical |
| `SysTreeView32` | Explorer | The folder tree in the Navigation Pane. | Critical |
| `DirectUIHWND` | Explorer | Used for various modern Explorer UI elements. | Moderate |
| `TrayNotifyWnd` | Start Menu | The notification area (system tray). | Moderate |

### Message Assumptions:
- `TaskbarCreated`: Registered message used to detect Explorer restarts.
- `WM_SYSCOMMAND` + `SC_TASKLIST`: Intercepted to override the Win-key.
- `OpenShellMenu.StartMenuMsg`: Custom IPC message between `.exe` and `.dll`.
- `SB_GETPARTS` / `SB_SETPARTS`: Assumes standard StatusBar behavior for modification.

---

## 6. Shell Hooks and Event Sources

| Hook/Event | Registration Method | Purpose | Criticality |
|------------|---------------------|---------|-------------|
| `WH_GETMESSAGE` | `SetWindowsHookEx` | Injected into Explorer's thread to intercept Win-key and other messages. | Critical |
| `WH_CBT` | `SetWindowsHookEx` | Intercepting window creation (`HCBT_CREATEWND`) to subclass Explorer or Dialogs. | Critical |
| `WH_KEYBOARD` | `SetWindowsHookEx` | Overriding Alt+D and Backspace in Explorer. | Moderate |
| `WH_MOUSE` | `SetWindowsHookEx` | Handling Win11 Start button interactions. | Critical (Win11) |
| `IAppVisibilityEvents` | `IAppVisibility::Advise` | Detecting Start Screen visibility changes. | Critical (Win8+) |
| `DWebBrowserEvents2` | `DispEventAdvise` | Detecting Explorer navigation (`NavigateComplete2`) or quit. | Critical |
| `TaskbarCreated` | `RegisterWindowMessage` | Re-hooking Explorer after it crashes/restarts. | Critical |

---

## 7. Integration Touchpoints (Explorer Coupling)

- **Process Injection**: `StartMenu.exe` is the watchdog that ensures `StartMenuDLL.dll` is always loaded in `explorer.exe`. It depends on finding the `Progman` window to identify the shell process.
- **Start Button Replacement**: Open-Shell doesn't just "add" a button; it either subclasses the existing one or places its own window exactly over it, intercepting all mouse and keyboard input.
- **Taskbar Skinning**: Accomplished by IAT-hooking GDI and Theme APIs *inside* the Explorer process. This means Open-Shell is actively modifying Explorer's internal drawing logic.
- **Explorer Control Subclassing**: The BHO finds the internal `SysTreeView32`, `Rebar`, and `Breadcrumb` controls by traversing the window hierarchy of `CabinetWClass` and applies subclassing. It depends on the internal layout of Explorer staying consistent.
- **Shell Copy Replacement**: Relies on `WH_CBT` to catch windows with class `#32770` created by `shell32.dll` and matching specific titles. It then uses MSAA to "read" the dialog's content.

---

## 8. Risk and Abstraction Classification

| Item | Classification | Abstraction Need | Risk |
|------|----------------|------------------|------|
| **IAT Hooking** | **Critical** | High | Most likely to break on new OS versions or Wine/ReactOS. |
| **Window Subclassing** | **Critical** | Medium | Foundation of the UI; needs a platform-agnostic wrapper. |
| **Hardcoded Class Names** | **Critical** | High | Explorer-specific; must be mapped for other environments. |
| **PIDL / Shell Namespace** | **Critical** | Low | Well-defined APIs, but need careful wrapping for mocks. |
| **MSAA / Accessibility** | **Deferred** | Low | Specific to "Classic Copy" feature. |
| **Immersive Shell COM** | **Critical** | High | Highly version-dependent (Win8 vs Win10 vs Win11). |
| **Win32 Message Hooks** | **Critical** | Medium | Core for Win-key interception and DLL injection. |

### Top 5 Abstraction Candidates:
1. **Shell Window Discovery**: Abstract `FindWindow` calls for `Shell_TrayWnd`, etc.
2. **Hook Manager**: A unified interface for `SetWindowsHookEx` and IAT patching.
3. **UI Subclasser**: A wrapper for `SetWindowSubclass` that can handle platform differences.
4. **Shell Item Provider**: An abstraction over PIDLs and `IShellFolder` for testing.
5. **Explorer Integration Interface**: Abstracting BHO and Toolbar registration.

## 9. Audit Annotations (Codex)

- `SetWindowsHookEx` <!-- VERIFIED: found in Src/StartMenu/StartMenu.cpp:101 and Src/StartMenu/StartMenuDLL/StartMenuDLL.cpp:3196 -->
- `SetWindowSubclass` <!-- VERIFIED: found in Src/StartMenu/StartMenuDLL/MenuContainer.cpp:4121 and Src/StartMenu/StartMenuDLL/StartMenuDLL.cpp:3153 -->
- `FindWindow/FindWindowEx` <!-- VERIFIED: found in Src/StartMenu/StartMenu.cpp:53 and Src/StartMenu/StartMenuDLL/StartMenuDLL.cpp:456 -->
- `RegisterWindowMessage` <!-- VERIFIED: found in Src/StartMenu/StartMenu.cpp:693 and Src/StartMenu/StartMenuDLL/MenuContainer.cpp:7596 -->
- `ChangeWindowMessageFilterEx` <!-- VERIFIED: dynamically loaded and called in Src/StartMenu/StartMenu.cpp:694-702 -->
- `RegisterHotKey` <!-- VERIFIED: found in Src/StartMenu/StartMenuDLL/StartMenuDLL.cpp:1180-1216 -->
- `SHAppBarMessage` <!-- VERIFIED: found in Src/StartMenu/StartMenuDLL/StartMenuDLL.cpp:605 and Src/StartMenu/StartMenuDLL/MenuContainer.cpp:8196 -->
- `SHGetIDListFromObject` <!-- VERIFIED: found in Src/StartMenu/StartMenuDLL/ItemManager.cpp:2399 -->
- `DwmExtendFrameIntoClientArea` <!-- VERIFIED: found in Src/StartMenu/StartMenuDLL/MenuContainer.cpp:4429 -->
- `DwmSetWindowAttribute` <!-- VERIFIED: found in Src/StartMenu/StartMenuDLL/StartButton.cpp:149-151 -->
- `IVirtualDesktopManager` <!-- MISSING FROM CODE: not found in Src/StartMenu via ripgrep -->
- `ITrayNotify` <!-- MISSING FROM CODE: not found in Src/StartMenu via ripgrep -->
- `IShellMenu` <!-- MISSING FROM CODE: not found in Src/StartMenu via ripgrep -->
- `IMenuBand` <!-- MISSING FROM CODE: not found in Src/StartMenu via ripgrep -->
- `IBandSite` <!-- MISSING FROM CODE: not found in Src/StartMenu via ripgrep -->

- Wine/ReactOS source trees were not present in this workspace snapshot. <!-- NEEDS ABSTRACTION: Wine/ReactOS compatibility claims require follow-up against actual forks referenced in task prompt. -->
- `RegisterShellHookWindow` usage is not present in current StartMenu sources. <!-- MISSING FROM CODE: not found in Src/StartMenu via ripgrep -->

// [CODEX] Last modified by: Codex
// [CODEX] Phase: 1
// [CODEX] Summary: Added source-backed verification annotations and missing API flags.
