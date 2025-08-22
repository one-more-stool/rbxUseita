#pragma once

#include <windows.h>
#include <vector>
#include "process_manager.h"
#include "localization.h"

// Forward declarations
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ImFont;

class UIManager {
public:
    UIManager();
    ~UIManager();

    bool Initialize(HWND hwnd);
    void Shutdown();
    void Render();
    
    bool ShouldClose() const { return m_shouldClose; }
    bool IsFadeOutComplete() const { return m_isClosing && m_windowAlpha <= 0.0f; }
    void StartFadeOut() { if (!m_isClosing) { m_isClosing = true; m_fadeOutStartTime = GetTickCount(); } }
    void SetWindowAlpha(HWND hwnd, float alpha);

private:
    void renderMainWindow();
    void updateRbxWindows();
    std::string getWindowTitle(DWORD processId);
    bool CreateDeviceD3D(HWND hWnd);
    void CleanupDeviceD3D();
    void CreateRenderTarget();
    void CleanupRenderTarget();

    // D3D11 resources
    ID3D11Device* m_pd3dDevice;
    ID3D11DeviceContext* m_pd3dDeviceContext;
    IDXGISwapChain* m_pSwapChain;
    ID3D11RenderTargetView* m_mainRenderTargetView;
    HWND m_hwnd;

    // Application state
    ProcessManager m_processManager;
    std::vector<ProcessInfo> m_processes;
    bool m_refreshProcesses;
    bool m_shouldClose;
    bool m_isShutdown;
    bool m_autoUnblock;
    DWORD m_lastUpdateTime;
    
    // Animation variables
    float m_windowAlpha; // Window fade animation
    float m_statusColorLerp; // Status color transition
    bool m_lastBlockingState; // Track blocking state changes
    DWORD m_lastStatusChange; // Time of last status change
    
    // Hover and dropdown animations
    float m_languageComboHover; // Language selector hover animation
    float m_exitButtonHover; // Exit button hover animation
    float m_checkboxHover; // Checkbox hover animation
    bool m_languageComboOpen; // Track if language dropdown is open
    DWORD m_lastHoverTime; // Time tracking for hover animations
    
    // Animation caching for performance
    struct AnimationCache {
        float cachedAlpha;
        float cachedComboHover;
        float cachedExitHover;
        float cachedCheckboxHover;
        DWORD lastUpdateTime;
        static const DWORD CACHE_DURATION = 16; // 60 FPS
    };
    AnimationCache m_animationCache;
    
    // Window fade animations
    bool m_isClosing; // Track if window is in closing animation
    DWORD m_fadeStartTime; // Start time for fade animations
    DWORD m_fadeOutStartTime; // Start time for fade-out animation
    
    // Window information
    struct RbxWindow {
        DWORD processId;
        std::string title;
        bool hasSingletonEvent;
        std::vector<HandleInfo> handles;
    };
    std::vector<RbxWindow> m_rbxWindows;

    // Window dragging
    bool m_isDragging;
    POINT m_dragStartPos;
    POINT m_windowStartPos;
    
    // Font Awesome support
    ImFont* m_fontAwesome;
};
