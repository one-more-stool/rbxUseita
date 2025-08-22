#include <windows.h>
#include <dwmapi.h>
#include "ui_manager.h"
#include "imgui.h"
#include "config.h"
#include "resource.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Global variables
UIManager g_uiManager;

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED) {
            // Handle resize if needed
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_KEYDOWN:
        if (wParam == VK_F4 && (GetKeyState(VK_MENU) & 0x8000)) { // Alt+F4
            // Start fade-out animation instead of immediate close
            g_uiManager.StartFadeOut();
            return 0;
        }
        break;
    case WM_DESTROY:
        g_uiManager.Shutdown();
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Main function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Create application window (borderless)
    WNDCLASSEX wc = { 
        sizeof(WNDCLASSEX), 
        CS_CLASSDC, 
        WndProc, 
        0L, 
        0L, 
        hInstance, 
        LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)), // Иконка окна
        LoadCursor(NULL, IDC_ARROW), 
        NULL, 
        NULL, 
        "rbxUseita", 
        LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1))  // Иконка приложения
    };
    RegisterClassEx(&wc);
    // Get configuration
    auto& config = Config::getInstance();
    
    // Get screen dimensions to center the window
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int windowWidth = config.windowWidth;
    int windowHeight = config.windowHeight;
    int posX = config.centerWindow ? (screenWidth - windowWidth) / 2 : 0;
    int posY = config.centerWindow ? (screenHeight - windowHeight) / 2 : 0;

    HWND hwnd = CreateWindowEx(WS_EX_LAYERED, wc.lpszClassName, "rbxUseita", WS_POPUP, posX, posY, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

    if (!hwnd) {
        MessageBoxA(NULL, "Failed to create window", "Error", MB_OK | MB_ICONERROR);
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Initialize UI Manager
    if (!g_uiManager.Initialize(hwnd)) {
        MessageBoxA(NULL, "Failed to initialize UI Manager", "Error", MB_OK | MB_ICONERROR);
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT && !g_uiManager.ShouldClose() && !g_uiManager.IsFadeOutComplete()) {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        g_uiManager.Render();
    }

    // Cleanup
    DestroyWindow(hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}
