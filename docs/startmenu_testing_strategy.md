# Start Menu Testing Strategy (Behavioral Reference Model)

This document outlines the strategy for validating Start Menu behavior using a normative behavioral reference model.

## 1. Objectives
- Define a **Behavioral Reference Model** that serves as the authoritative specification of Start Menu behavior.
- Ensure regression safety for future refactoring and porting (ReactOS/Wine).
- Provide a target for production implementation validation in Phase 3.

## 2. Testing Approach (Phase 2)

### 2.1. Behavioral Reference Model
In Phase 2, the tests are implemented as a **headless simulator**. This simulator encodes the *expected behavior* based on current observations and documentation.

**Note:** These tests are NOT yet bound to the actual production code. They define *what* should happen, not *how* the current C++ implementation does it.

### 2.2. Mocking and Simulation
The simulator uses mock triggers and state tracking to validate the behavioral contract:
- **Triggers:** Simulated events like `PressWinKey()`, `ClickStartButton()`, `TypeInSearch()`.
- **State Tracking:** Monitoring transitions between `Closed`, `OpenRoot`, `OpenSubmenu`, and `Searching`.
- **Side Effects:** Tracking observable outcomes like `WasAppLaunched()`.

## 3. Scope of Testing

### 3.1. Behaviors for Future Regression Enforcement (Phase 3)
Once abstractions are introduced in Phase 3, the following behaviors will be directly validated against the production code:
- **Win-Key Interception:** Transition from `Closed` to `OpenRoot` upon receiving `WM_HOTKEY`.
- **Start Button Subclassing:** Activation of the menu via `WM_LBUTTONDOWN` on the taskbar/start button window.
- **Menu Lifecycle:** Correct closing of the menu on focus loss (`WA_INACTIVE`) or application launch.
- **Search Integration:** Proper state transition to `Searching` when text input is received.

### 3.2. Headless Validation
The current simulator validates the logic of:
- State transitions (Closed -> Open -> SubmenuOpen -> Closed).
- Escape key nesting (closing submenus before the root menu).
- Search state lifecycle.

## 4. Known Limitations
- **Visual Rendering:** Since tests are headless, pixel-perfect rendering and skinning are out of scope for this model.
- **Explorer Dependency:** The model assumes standard Explorer behavior (e.g., taskbar existence, message flow) which will be mocked in Phase 3.

## 5. Ambiguity and Underspecified Behaviors
- **Multi-Monitor Behavior:** The exact behavior when clicking a secondary taskbar versus the primary one is currently simplified in the reference model.
- **Touch Interaction:** While some touch hooks exist in production, the reference model currently prioritizes mouse/keyboard interactions.
- **Win10 Tablet Mode:** The transition logic for tablet mode is identified but not fully exercised in the basic reference model.
