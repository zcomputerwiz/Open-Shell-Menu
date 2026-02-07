# Start Menu Behavioral Inventory

This document lists the observable behaviors of the Open-Shell Start Menu, their triggers, preconditions, and expected outcomes.

## 1. Start Menu Lifecycle

### 1.1. Open Start Menu via Windows Key
- **Trigger:** Pressing the Windows key (or Ctrl+Esc).
- **Preconditions:** Start Menu is currently closed.
- **Expected Outcome:** The Start Menu opens on the primary taskbar.
- **Code Locations:**
    - `Src/StartMenu/StartMenuDLL/StartMenuDLL.cpp`: ~3763 (WM_HOTKEY handler)
    - `Src/StartMenu/StartMenuDLL/MenuContainer.cpp`: `CMenuContainer::ToggleStartMenu`

### 1.2. Close Start Menu via Windows Key
- **Trigger:** Pressing the Windows key while the Start Menu is open.
- **Preconditions:** Start Menu is currently open.
- **Expected Outcome:** The Start Menu closes.
- **Code Locations:**
    - `Src/StartMenu/StartMenuDLL/StartMenuDLL.cpp`: ~3763 (WM_HOTKEY handler)
    - `Src/StartMenu/StartMenuDLL/MenuContainer.cpp`: ~7559 (`CMenuContainer::ToggleStartMenu` calling `CloseStartMenu`)

### 1.3. Toggle Start Menu via Start Button Click
- **Trigger:** Left-clicking the Start Button.
- **Preconditions:** None.
- **Expected Outcome:** If closed, it opens. If open, it closes.
- **Code Locations:**
    - `Src/StartMenu/StartMenuDLL/StartMenuDLL.cpp`: ~3858 (Subclass hook handling `WM_LBUTTONDOWN`)
    - `Src/StartMenu/StartMenuDLL/MenuContainer.cpp`: `CMenuContainer::ToggleStartMenu`

### 1.4. Close Start Menu on Deactivation
- **Trigger:** Clicking outside the Start Menu or switching focus to another application.
- **Preconditions:** Start Menu is open.
- **Expected Outcome:** The Start Menu closes.
- **Code Locations:**
    - `Src/StartMenu/StartMenuDLL/MenuContainer.cpp`: ~6411 (`CMenuContainer::OnActivate` handling `WA_INACTIVE`)

### 1.5. Close Start Menu via Escape Key
- **Trigger:** Pressing the `Esc` key.
- **Preconditions:** Start Menu is open and has focus.
- **Expected Outcome:** The top-most menu level closes. If at the root level, the entire menu closes.
- **Code Locations:**
    - `Src/StartMenu/StartMenuDLL/MenuContainer.cpp`: ~5929 (`CMenuContainer::OnKeyDown`)

### 1.6. Single Instance Enforcement
- **Trigger:** Starting `StartMenu.exe`.
- **Preconditions:** Another instance of `StartMenu.exe` is already running on the same desktop.
- **Expected Outcome:** The new instance detects the existing one via a mutex and exits, potentially toggling the menu in the existing instance.
- **Code Locations:**
    - `Src/StartMenu/StartMenu.cpp`: ~650 (Mutex creation and check)

## 2. Menu Navigation and Interaction

### 2.1. Open Submenu on Hover
- **Trigger:** Hovering the mouse over a folder item for a certain duration.
- **Preconditions:** Start Menu is open.
- **Expected Outcome:** A submenu for the hovered folder opens.
- **Code Locations:**
    - `Src/StartMenu/StartMenuDLL/MenuContainer.cpp`: `CMenuContainer::OnTimer` (handling `TIMER_HOVER`)
    - `Src/StartMenu/StartMenuDLL/MenuContainer.cpp`: `CMenuContainer::OpenSubMenu`

### 2.2. Launch Application
- **Trigger:** Left-clicking an application item or pressing `Enter` on a selected item.
- **Preconditions:** Start Menu is open.
- **Expected Outcome:** The application is launched and the Start Menu closes.
- **Code Locations:**
    - `Src/StartMenu/StartMenuDLL/MenuCommands.cpp`: ~1392 (`CMenuContainer::ActivateItem` with `ACTIVATE_EXECUTE`)
    - `Src/StartMenu/StartMenuDLL/MenuCommands.cpp`: ~1654 (Closing menus after execution)

### 2.3. Keyboard Navigation
- **Trigger:** Pressing arrow keys (`Up`, `Down`, `Left`, `Right`).
- **Preconditions:** Start Menu is open and has focus.
- **Expected Outcome:** The selection moves between items or opens/closes submenus.
- **Code Locations:**
    - `Src/StartMenu/StartMenuDLL/MenuContainer.cpp`: `CMenuContainer::OnKeyDown`

## 3. Search Functionality

### 3.1. Initiate Search
- **Trigger:** Typing characters while the Start Menu (or specifically the search box) has focus.
- **Preconditions:** Start Menu is open.
- **Expected Outcome:** The search box is populated, and search results are displayed in the menu.
- **Code Locations:**
    - `Src/StartMenu/StartMenuDLL/MenuContainer.cpp`: `CMenuContainer::OnEditChange`
    - `Src/StartMenu/StartMenuDLL/SearchManager.cpp`

## 4. Specialized Interactions

### 4.1. Shift+Click on Start Button
- **Trigger:** Clicking the Start Button while holding the `Shift` key.
- **Preconditions:** None.
- **Expected Outcome:** Opens the original Windows Start Menu (behavior configurable).
- **Code Locations:**
    - `Src/StartMenu/StartMenuDLL/StartMenuDLL.cpp`: ~3848 (Handling `ShiftClick`)

### 4.2. Right-Click on Start Button
- **Trigger:** Right-clicking the Start Button.
- **Preconditions:** None.
- **Expected Outcome:** Shows the Open-Shell context menu for the Start Button (settings, exit, etc.).
- **Code Locations:**
    - `Src/StartMenu/StartMenuDLL/StartMenuDLL.cpp`: ~4063 (Handling `WM_RBUTTONUP` on Start Button)

---

## 5. Ambiguities and Underspecified Behaviors

### 5.1. Multi-Monitor Taskbar Interaction
- **Ambiguity:** It is not explicitly clear if the menu always opens on the monitor where the click occurred if multiple taskbars are present, or if it defaults to the primary monitor in certain scenarios (e.g., Win-key press).
- **Code Location:** `Src/StartMenu/StartMenuDLL/MenuContainer.cpp`: `CalculateCorner` and `s_MenuMonitor`.

### 5.2. Drag and Drop onto Start Button
- **Ambiguity:** The behavior when dragging a file *onto* the Start Button (to pin it or open it) is implemented via `IDropTarget` but the exact visual feedback and state transitions are complex and underspecified in the inventory.
- **Code Location:** `Src/StartMenu/StartMenuDLL/DragDrop.cpp`.

### 5.3. Immersive Shell / WinRT Interactions
- **Ambiguity:** Interactions with the "Immersive Shell" (Win8/10 Start Screen/Menu) are handled via COM interfaces (`IAppVisibility`, `IImmersiveShell`). The exact behavior when both menus might try to open simultaneously is highly OS-version dependent and not fully specified.
- **Code Location:** `Src/StartMenu/StartMenuDLL/StartMenuDLL.cpp`: ~3050.
