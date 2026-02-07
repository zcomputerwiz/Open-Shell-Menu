/**
 * BEHAVIORAL REFERENCE MODEL TESTS - PHASE 2
 *
 * These tests validate the Behavioral Reference Model (simulator)
 * which serves as the normative specification for the Start Menu.
 *
 * NOTE: These tests validate the specification, not the production code yet.
 */

#include "StartMenuTestHarness.h"
#include <iostream>
#include <cassert>

/**
 * Validates that pressing the Windows Key toggles the menu state.
 * Reference: Behavioral Contract 3.0 / Inventory 1.1 & 1.2
 */
void TestWinKeyToggle() {
    StartMenuTestHarness harness;
    assert(harness.GetState() == StartMenuState::Closed);

    harness.PressWinKey();
    assert(harness.GetState() == StartMenuState::OpenRoot);

    harness.PressWinKey();
    assert(harness.GetState() == StartMenuState::Closed);
    std::cout << "TestWinKeyToggle passed." << std::endl;
}

/**
 * Validates that clicking the Start Button toggles the menu state.
 * Reference: Behavioral Contract 3.0 / Inventory 1.3
 */
void TestStartButtonClickToggle() {
    StartMenuTestHarness harness;
    assert(harness.GetState() == StartMenuState::Closed);

    harness.ClickStartButton();
    assert(harness.GetState() == StartMenuState::OpenRoot);

    harness.ClickStartButton();
    assert(harness.GetState() == StartMenuState::Closed);
    std::cout << "TestStartButtonClickToggle passed." << std::endl;
}

/**
 * Validates that the menu closes when losing focus.
 * Reference: Behavioral Contract 3.0 / Inventory 1.4
 */
void TestDeactivationClosesMenu() {
    StartMenuTestHarness harness;
    harness.PressWinKey();
    assert(harness.GetState() == StartMenuState::OpenRoot);

    harness.ClickOutside();
    assert(harness.GetState() == StartMenuState::Closed);
    std::cout << "TestDeactivationClosesMenu passed." << std::endl;
}

/**
 * Validates the Escape key behavior at different menu levels.
 * Reference: Behavioral Contract 3.0 / Inventory 1.5
 */
void TestEscapeKeyBehavior() {
    StartMenuTestHarness harness;

    // Root level Esc
    harness.PressWinKey();
    harness.PressEscape();
    assert(harness.GetState() == StartMenuState::Closed);

    // Submenu level Esc
    harness.PressWinKey();
    harness.HoverFolder();
    assert(harness.GetState() == StartMenuState::OpenSubmenu);
    harness.PressEscape();
    assert(harness.GetState() == StartMenuState::OpenRoot);
    harness.PressEscape();
    assert(harness.GetState() == StartMenuState::Closed);

    std::cout << "TestEscapeKeyBehavior passed." << std::endl;
}

/**
 * Validates that launching an application closes the menu.
 * Reference: Behavioral Contract 3.0 / Inventory 2.2
 */
void TestLaunchAppClosesMenu() {
    StartMenuTestHarness harness;
    harness.PressWinKey();
    assert(harness.GetState() == StartMenuState::OpenRoot);

    harness.LaunchApplication();
    assert(harness.WasAppLaunched());
    assert(harness.GetState() == StartMenuState::Closed);
    std::cout << "TestLaunchAppClosesMenu passed." << std::endl;
}

/**
 * Validates search state transitions.
 * Reference: Behavioral Contract 3.0 / Inventory 3.1
 */
void TestSearchTransitions() {
    StartMenuTestHarness harness;
    harness.PressWinKey();

    harness.TypeInSearch("calc");
    assert(harness.GetState() == StartMenuState::Searching);
    assert(harness.GetSearchText() == "calc");

    harness.TypeInSearch("");
    assert(harness.GetState() == StartMenuState::OpenRoot);

    std::cout << "TestSearchTransitions passed." << std::endl;
}

int main() {
    std::cout << "Running Start Menu Behavioral Reference Model Tests..." << std::endl;
    try {
        TestWinKeyToggle();
        TestStartButtonClickToggle();
        TestDeactivationClosesMenu();
        TestEscapeKeyBehavior();
        TestLaunchAppClosesMenu();
        TestSearchTransitions();
        std::cout << "\nAll behavioral reference model tests passed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
