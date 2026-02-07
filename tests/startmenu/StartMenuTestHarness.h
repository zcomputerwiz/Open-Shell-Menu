#pragma once

#include <string>
#include <vector>
#include <memory>

/**
 * BEHAVIORAL REFERENCE MODEL - PHASE 2
 *
 * This file is part of the Start Menu Behavioral Reference Model.
 * It encodes the normative behavioral specifications for the Open-Shell Start Menu.
 *
 * NOTE: This is a SIMULATOR. It is not yet bound to the actual production code in Src/.
 * In Phase 3, production code will be refactored to be validated against this model.
 */

// Mock Win32/Shell constants and types for the reference model
typedef void* HWND;
typedef unsigned int UINT;
typedef unsigned long long WPARAM;
typedef long long LPARAM;

#define WM_NULL 0x0000
#define WM_CREATE 0x0001
#define WM_DESTROY 0x0002
#define WM_CLOSE 0x0010
#define WM_ACTIVATE 0x0006
#define WA_INACTIVE 0
#define WM_KEYDOWN 0x0100
#define WM_LBUTTONDOWN 0x0201
#define WM_HOTKEY 0x0312

#define VK_ESCAPE 0x1B
#define VK_RETURN 0x0D
#define VK_UP     0x26
#define VK_DOWN   0x28

#define LOWORD(l) ((unsigned short)((unsigned long)(l) & 0xffff))

/**
 * Start Menu States as defined in docs/startmenu_behavior_contract.md
 */
enum class StartMenuState {
    Closed,
    OpenRoot,
    OpenSubmenu,
    Searching
};

/**
 * The StartMenuTestHarness provides a mockable environment for testing
 * Start Menu behavioral logic. In Phase 2, this serves as a simulator
 * that encodes the current documented behavior of Open-Shell.
 */
class StartMenuTestHarness {
public:
    StartMenuTestHarness() : m_State(StartMenuState::Closed), m_AppLaunched(false) {}

    // --- Actions (Triggers from Inventory) ---

    void PressWinKey() {
        // Behavioral Inventory 1.1 & 1.2: Windows key toggles the menu
        HandleMessage(WM_HOTKEY, 0, 0);
    }

    void ClickStartButton() {
        // Behavioral Inventory 1.3: Click on start button toggles the menu
        HandleMessage(WM_LBUTTONDOWN, 0, 0);
    }

    void ClickOutside() {
        // Behavioral Inventory 1.4: Menu closes on deactivation
        HandleMessage(WM_ACTIVATE, WA_INACTIVE, 0);
    }

    void PressEscape() {
        // Behavioral Inventory 1.5: Escape key closes the top-most menu level
        HandleMessage(WM_KEYDOWN, VK_ESCAPE, 0);
    }

    void HoverFolder() {
        // Behavioral Inventory 2.1: Hovering opens a submenu
        if (m_State == StartMenuState::OpenRoot || m_State == StartMenuState::OpenSubmenu) {
            m_State = StartMenuState::OpenSubmenu;
        }
    }

    void LaunchApplication() {
        // Behavioral Inventory 2.2: Launching an app closes the menu
        m_AppLaunched = true;
        m_State = StartMenuState::Closed;
    }

    void TypeInSearch(const std::string& text) {
        // Behavioral Inventory 3.1: Typing initiates/updates search
        if (m_State == StartMenuState::OpenRoot || m_State == StartMenuState::Searching) {
            m_SearchText = text;
            m_State = text.empty() ? StartMenuState::OpenRoot : StartMenuState::Searching;
        }
    }

    // --- State Queries ---

    StartMenuState GetState() const { return m_State; }
    bool WasAppLaunched() const { return m_AppLaunched; }
    std::string GetSearchText() const { return m_SearchText; }

private:
    /**
     * Internal state machine that encodes behaviors documented in the contract.
     */
    void HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
            case WM_HOTKEY:
            case WM_LBUTTONDOWN:
                if (m_State == StartMenuState::Closed) {
                    m_State = StartMenuState::OpenRoot;
                } else {
                    m_State = StartMenuState::Closed;
                }
                break;
            case WM_ACTIVATE:
                if (LOWORD(wParam) == WA_INACTIVE) {
                    m_State = StartMenuState::Closed;
                }
                break;
            case WM_KEYDOWN:
                if (wParam == VK_ESCAPE) {
                    if (m_State == StartMenuState::OpenSubmenu) {
                        m_State = StartMenuState::OpenRoot;
                    } else {
                        m_State = StartMenuState::Closed;
                    }
                }
                break;
            case WM_CLOSE:
                m_State = StartMenuState::Closed;
                break;
        }
    }

    StartMenuState m_State;
    bool m_AppLaunched;
    std::string m_SearchText;
};
