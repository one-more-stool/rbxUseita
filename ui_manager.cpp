#include "ui_manager.h"
#include "font_data.h"
#include "fa_solid_data.h"
#include "localization.h"
#include "theme.h"
#include "config.h"
#include <d3d11.h>
#include <algorithm>

// ImGui includes
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

UIManager::UIManager() 
    : m_pd3dDevice(nullptr)
    , m_pd3dDeviceContext(nullptr)
    , m_pSwapChain(nullptr)
    , m_mainRenderTargetView(nullptr)
    , m_hwnd(nullptr)
    , m_refreshProcesses(true)
    , m_shouldClose(false)
    , m_isShutdown(false)
    , m_isDragging(false)
    , m_dragStartPos{0, 0}
    , m_windowStartPos{0, 0}
    , m_fontAwesome(nullptr)
    , m_autoUnblock(Config::getInstance().autoUnblock)
    , m_lastUpdateTime(0)
    , m_windowAlpha(0.0f)
    , m_statusColorLerp(0.0f)
    , m_lastBlockingState(false)
    , m_lastStatusChange(GetTickCount())
    , m_languageComboHover(0.0f)
    , m_exitButtonHover(0.0f)
    , m_checkboxHover(0.0f)
    , m_languageComboOpen(false)
    , m_lastHoverTime(GetTickCount())
    , m_isClosing(false)
    , m_fadeStartTime(0)
    , m_fadeOutStartTime(0)
    , m_animationCache{0.0f, 0.0f, 0.0f, 0.0f, 0}
{
    // Initialize localization with detected/saved language
    Localization::getInstance().setLanguage(Config::getInstance().language);
}

UIManager::~UIManager() {
    Shutdown();
}

bool UIManager::Initialize(HWND hwnd) {
    // Save window handle
    m_hwnd = hwnd;
    
    // Create D3D device
    if (!CreateDeviceD3D(hwnd)) {
        return false;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    // Disable imgui.ini file creation
    io.IniFilename = nullptr;
    
    // Use system font with Cyrillic support instead of Lato
    ImFontConfig font_config;
    font_config.SizePixels = 16.0f;
    
    // Add Cyrillic glyph ranges
    static const ImWchar cyrillic_ranges[] = {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
        0x2DE0, 0x2DFF, // Cyrillic Extended-A
        0xA640, 0xA69F, // Cyrillic Extended-B
        0,
    };
    
    // Try to load MS Sans Serif with Cyrillic support
    ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\micross.ttf", 16.0f, &font_config, cyrillic_ranges);
    if (!font) {
        // Fallback to Segoe UI
        font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 16.0f, &font_config, cyrillic_ranges);
        if (!font) {
            // Final fallback to default font with built-in Cyrillic ranges
            font = io.Fonts->AddFontDefault(&font_config);
            io.Fonts->Build(); // Rebuild font atlas
            OutputDebugStringA("Using default font with Cyrillic support.\n");
        } else {
            io.FontDefault = font;
            OutputDebugStringA("Loaded Segoe UI font with Cyrillic support.\n");
        }
    } else {
        io.FontDefault = font;
        OutputDebugStringA("Loaded MS Sans Serif font with Cyrillic support.\n");
    }
    
    // Load Font Awesome for icons from embedded data
    ImFontConfig fa_config;
    fa_config.MergeMode = true; // Merge with existing font
    fa_config.PixelSnapH = true;
    fa_config.FontDataOwnedByAtlas = true; // ImGui will manage the memory
    
    // Font Awesome icon ranges (Solid icons)
    static const ImWchar fa_ranges[] = {
        0xf000, 0xf6ff, // Font Awesome Free Solid
        0,
    };
    
    // Load Font Awesome from embedded data
    void* faDataCopy = malloc(fa_solidFontSize);
    if (faDataCopy) {
        memcpy(faDataCopy, fa_solidFontData, fa_solidFontSize);
        m_fontAwesome = io.Fonts->AddFontFromMemoryTTF(faDataCopy, fa_solidFontSize, 16.0f, &fa_config, fa_ranges);
        if (m_fontAwesome) {
            OutputDebugStringA("✅ Loaded embedded Font Awesome Solid icons.\n");
        } else {
            free(faDataCopy);
            OutputDebugStringA("❌ Failed to load embedded Font Awesome.\n");
        }
    } else {
        OutputDebugStringA("❌ Could not allocate memory for Font Awesome data.\n");
    }
    
    if (!m_fontAwesome) {
        OutputDebugStringA("Font Awesome not available. Icons will not be displayed.\n");
    }

    // Setup custom theme
    Theme::ApplyDarkTheme();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(m_pd3dDevice, m_pd3dDeviceContext);

    // Initialize fade-in animation start time
    m_fadeStartTime = GetTickCount();

    return true;
}

void UIManager::SetWindowAlpha(HWND hwnd, float alpha) {
    // Convert float alpha (0.0-1.0) to BYTE (0-255)
    BYTE alphaValue = (BYTE)(alpha * 255);
    
    // Set window transparency
    SetLayeredWindowAttributes(hwnd, 0, alphaValue, LWA_ALPHA);
}

void UIManager::Shutdown() {
    if (m_isShutdown) {
        return; // Already shutdown
    }
    
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    
    m_isShutdown = true;
}

// Helper function for smooth interpolation
float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

// Helper function for smooth easing
float easeInOut(float t) {
    return t * t * (3.0f - 2.0f * t);
}

void UIManager::Render() {
    // Auto-refresh every second (but not while dragging)
    DWORD currentTime = GetTickCount();
    
    // Window fade animations (fade-in and fade-out)
    if (m_isClosing) {
        // Fade-out animation (300ms)
        float fadeOutTime = (currentTime - m_fadeOutStartTime) / 300.0f;
        m_windowAlpha = 1.0f - easeInOut(fadeOutTime > 1.0f ? 1.0f : fadeOutTime);
        
        // If fade-out is complete, signal to close the window
        if (fadeOutTime >= 1.0f) {
            m_windowAlpha = 0.0f;
            // Window will be closed by the main loop
        }
    } else {
        // Fade-in animation (500ms)
        float fadeInTime = (currentTime - m_fadeStartTime) / 500.0f;
        if (fadeInTime > 1.0f) {
            m_windowAlpha = 1.0f;
        } else {
            m_windowAlpha = easeInOut(fadeInTime);
        }
    }
    
    // Apply window transparency
    if (m_hwnd) {
        SetWindowAlpha(m_hwnd, m_windowAlpha);
    }
    
    // Status color animation (calculate early to use currentTime)
    float transitionTime = (currentTime - m_lastStatusChange) / 300.0f; // 300ms transition
    m_statusColorLerp = easeInOut(transitionTime > 1.0f ? 1.0f : transitionTime);
    

    
    if (!m_isDragging && currentTime - m_lastUpdateTime > 1000) { // 1 second
        m_processes = m_processManager.findRbxProcesses();
        updateRbxWindows();
        m_lastUpdateTime = currentTime;
        
        // Auto-unblock if enabled
        if (m_autoUnblock) {
            for (auto& process : m_processes) {
                if (!process.handles.empty()) {
                    m_processManager.closeProcessHandles(process);
                }
            }
        }
    }
    
    // Manual refresh if requested
    if (m_refreshProcesses) {
        m_processes = m_processManager.findRbxProcesses();
        updateRbxWindows();
        m_refreshProcesses = false;
    }

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    renderMainWindow();

    // Rendering
    ImGui::Render();
    const float clear_color[4] = { 0.0f, 0.0f, 0.0f, 0.0f }; // Transparent background
    m_pd3dDeviceContext->OMSetRenderTargets(1, &m_mainRenderTargetView, nullptr);
    m_pd3dDeviceContext->ClearRenderTargetView(m_mainRenderTargetView, clear_color);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    m_pSwapChain->Present(0, 0); // Present immediately, no vsync
}

void UIManager::renderMainWindow() {
    // Get IO for display size
    ImGuiIO& io = ImGui::GetIO();
    
    // Set window to fill entire display
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(io.DisplaySize);
    
    // Remove window decorations
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | 
                                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    
    // Begin main ImGui window (no alpha style needed - using real window transparency)
    ImGui::Begin("MainDockSpace", nullptr, window_flags);
    
    // Draw colored border around window
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();
    ImU32 border_color = IM_COL32(255, 255, 255, 20); // RGBA: 255,255,255,10
    float border_thickness = 1.0f;
    
    draw_list->AddRect(
        window_pos, 
        ImVec2(window_pos.x + window_size.x, window_pos.y + window_size.y), 
        border_color, 
        0.0f, // No corner rounding
        0, 
        border_thickness
    );
    
    ImGui::PopStyleVar(3); // Pop WindowPadding, WindowBorderSize, WindowRounding
    
    // Add some padding back for content
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
    
    // Custom title bar with dragging capability (no hover effects)
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.08f)); // White with 20/255 alpha
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.08f)); // Same as normal (no hover effect)
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 1.0f, 1.0f, 0.08f)); // Same as normal (no click effect)
    
    // Title bar (non-clickable for dragging) - simple text without icon
    ImGui::Button("rbxUseita", ImVec2(-40, 30)); // Make room for exit button
    
    // Handle window dragging when hovering over title and clicking
    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        m_isDragging = true;
        GetCursorPos(&m_dragStartPos);
        HWND hwnd = GetActiveWindow();
        RECT rect;
        GetWindowRect(hwnd, &rect);
        m_windowStartPos.x = rect.left;
        m_windowStartPos.y = rect.top;
    }
    
    ImGui::PopStyleColor(3); // Reset title button style
    
    // Exit button positioned absolutely in top-right corner
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImGui::SetCursorPos(ImVec2(windowSize.x - 40, 0)); // Position at top-right
    
    // Calculate hover animation for exit button
    static float lastExitHover = 0.0f;
    static DWORD exitHoverStartTime = GetTickCount();
    
    if (m_exitButtonHover != lastExitHover) {
        lastExitHover = m_exitButtonHover;
        exitHoverStartTime = GetTickCount();
    }
    
    float exitHoverTime = (GetTickCount() - exitHoverStartTime) / 200.0f; // 200ms animation
    float exitHoverProgress = easeInOut(exitHoverTime > 1.0f ? 1.0f : exitHoverTime);
    float currentExitHover = lerp(lastExitHover == 1.0f ? 0.0f : 1.0f, lastExitHover, exitHoverProgress);
    
    Theme::ApplyAnimatedExitButtonStyle(currentExitHover); // Animated exit button style
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f); // No rounding for exit button
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f)); // No padding for exit button
    if (m_fontAwesome) {
        ImGui::PushFont(m_fontAwesome);
        if (ImGui::Button("\xef\x80\x8d", ImVec2(40, 30))) { // fa-times icon
            if (!m_isClosing) {
                m_isClosing = true;
                m_fadeOutStartTime = GetTickCount();
            }
        }
        ImGui::PopFont();
    } else {
        if (ImGui::Button("X", ImVec2(40, 30))) {
            if (!m_isClosing) {
                m_isClosing = true;
                m_fadeOutStartTime = GetTickCount();
            }
        }
    }
    
    // Track exit button hover state (after creating the element)
    bool isExitHovered = ImGui::IsItemHovered();
    
    // Update exit button hover animation
    if (isExitHovered && m_exitButtonHover < 1.0f) {
        m_exitButtonHover = 1.0f;
        m_lastHoverTime = GetTickCount();
    } else if (!isExitHovered && m_exitButtonHover > 0.0f) {
        m_exitButtonHover = 0.0f;
        m_lastHoverTime = GetTickCount();
    }
    
    ImGui::PopStyleVar(2); // Pop FramePadding, FrameRounding
    Theme::ResetButtonStyle(); // Reset exit button style
    
    if (m_isDragging) {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            POINT currentPos;
            GetCursorPos(&currentPos);
            int deltaX = currentPos.x - m_dragStartPos.x;
            int deltaY = currentPos.y - m_dragStartPos.y;
            
            HWND hwnd = GetActiveWindow();
            SetWindowPos(hwnd, NULL, 
                m_windowStartPos.x + deltaX, 
                m_windowStartPos.y + deltaY, 
                0, 0, SWP_NOSIZE | SWP_NOZORDER);
        } else {
            m_isDragging = false;
        }
    }
    

    
    // Main content area (no background, no border)
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Transparent background
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f); // No border
    ImGui::BeginChild("Content", ImVec2(0, -20), false);
    
    // Calculate content height for vertical centering
    float windowHeight = ImGui::GetWindowSize().y;
    float titleBarHeight = 30.0f; // Height of title bar
    float availableHeight = windowHeight - titleBarHeight - 20.0f; // Subtract title and bottom margin
    float contentHeight = 60.0f; // Approximate height of our content (2 lines + spacing)
    float topPadding = (availableHeight - contentHeight) * 0.5f;
    if (topPadding < 8.0f) topPadding = 8.0f; // Minimum top padding
    
    ImGui::Dummy(ImVec2(0.0f, topPadding)); // Dynamic top padding for centering
    ImGui::Indent(16.0f); // Left padding
    
    const auto& strings = Localization::getInstance().getStrings();
    
    // Language selector and auto-unblock in one line
    ImGui::AlignTextToFramePadding(); // Align text to combo box height
    if (m_fontAwesome) {
        ImGui::PushFont(m_fontAwesome);
        ImGui::Text("\xef\x82\xac"); // fa-globe icon
        ImGui::PopFont();
    } else {
        ImGui::Text("Language:");
    }
    ImGui::SameLine();
    
    const char* languages[] = { 
        "English", "Русский", "Suomi", "Español", "Français", "Deutsch", "Italiano", "Português", 
        "Polski", "Nederlands", "Svenska", "Norsk", "Dansk", "Čeština", "Magyar", "Română", 
        "Български", "Українська", "Türkçe", "Tiếng Việt", "Bahasa Indonesia", "Bahasa Melayu", 
        "Ελληνικά", "Hrvatski", "Slovenčina", "Slovenščina", "Lietuvių", "Latviešu", "Eesti"
    };
    const char* languageCodes[] = { 
        "en", "ru", "fi", "es", "fr", "de", "it", "pt", "pl", "nl", "sv", "no", "da", "cs", "hu", "ro", 
        "bg", "uk", "tr", "vi", "id", "ms", "el", "hr", "sk", "sl", "lt", "lv", "et"
    };
    static int currentLang = 0;
    
    // Find current language index
    std::string currentLanguage = Localization::getInstance().getLanguage();
    for (int i = 0; i < 29; i++) {
        if (currentLanguage == languageCodes[i]) {
            currentLang = i;
            break;
        }
    }
    
    // Language selector with hover animation
    ImGui::SetNextItemWidth(120); // Set smaller width for combo
    
    // Calculate hover animation for combo
    static float lastComboHover = 0.0f;
    static DWORD comboHoverStartTime = GetTickCount();
    
    if (m_languageComboHover != lastComboHover) {
        lastComboHover = m_languageComboHover;
        comboHoverStartTime = GetTickCount();
    }
    
    float hoverTime = (GetTickCount() - comboHoverStartTime) / 200.0f; // 200ms animation
    float hoverProgress = easeInOut(hoverTime > 1.0f ? 1.0f : hoverTime);
    float currentComboHover = lerp(lastComboHover == 1.0f ? 0.0f : 1.0f, lastComboHover, hoverProgress);
    
    // Apply hover effect to combo with animation
    bool appliedHoverStyle = (currentComboHover > 0.01f);
    if (appliedHoverStyle) {
        // Interpolate colors based on hover progress
        float baseR = 0.18f, baseG = 0.18f, baseB = 0.22f; // Base FrameBg color
        float hoverR = 0.25f, hoverG = 0.25f, hoverB = 0.30f; // Hover FrameBg color
        
        float animR = lerp(baseR, hoverR, currentComboHover);
        float animG = lerp(baseG, hoverG, currentComboHover);
        float animB = lerp(baseB, hoverB, currentComboHover);
        
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(animR, animG, animB, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(animR + 0.05f, animG + 0.05f, animB + 0.05f, 1.0f));
    }
    
    if (ImGui::Combo("##language", &currentLang, languages, IM_ARRAYSIZE(languages))) {
        Localization::getInstance().setLanguage(languageCodes[currentLang]);
        // Save language to config
        Config::getInstance().language = languageCodes[currentLang];
        Config::getInstance().saveToFile();
    }
    
    // Pop styles if they were applied
    if (appliedHoverStyle) {
        ImGui::PopStyleColor(2);
    }
    
    // Track combo state for animation (after creating the element)
    bool isComboHovered = ImGui::IsItemHovered();
    bool isComboOpen = ImGui::IsPopupOpen("##language");
    
    // Update hover animation
    if (isComboHovered && m_languageComboHover < 1.0f) {
        m_languageComboHover = 1.0f;
        m_lastHoverTime = GetTickCount();
    } else if (!isComboHovered && m_languageComboHover > 0.0f) {
        m_languageComboHover = 0.0f;
        m_lastHoverTime = GetTickCount();
    }
    
    // Auto-unblock checkbox on same line
    ImGui::SameLine();
    ImGui::Spacing();
    ImGui::SameLine();
    
    // Calculate hover animation for checkbox
    static float lastCheckboxHover = 0.0f;
    static DWORD checkboxHoverStartTime = GetTickCount();
    
    if (m_checkboxHover != lastCheckboxHover) {
        lastCheckboxHover = m_checkboxHover;
        checkboxHoverStartTime = GetTickCount();
    }
    
    float checkboxHoverTime = (GetTickCount() - checkboxHoverStartTime) / 200.0f; // 200ms animation
    float checkboxHoverProgress = easeInOut(checkboxHoverTime > 1.0f ? 1.0f : checkboxHoverTime);
    float currentCheckboxHover = lerp(lastCheckboxHover == 1.0f ? 0.0f : 1.0f, lastCheckboxHover, checkboxHoverProgress);
    
    // Apply animated checkbox style
    bool appliedCheckboxStyle = (currentCheckboxHover > 0.01f);
    if (appliedCheckboxStyle) {
        Theme::ApplyAnimatedCheckboxStyle(currentCheckboxHover);
    }
    
    if (ImGui::Checkbox(strings.autoUnblock.c_str(), &m_autoUnblock)) {
        // Save auto-unblock setting to config
        Config::getInstance().autoUnblock = m_autoUnblock;
        Config::getInstance().saveToFile();
    }
    
    // Track checkbox hover state (after creating the element)
    bool isCheckboxHovered = ImGui::IsItemHovered();
    
    // Update checkbox hover animation
    if (isCheckboxHovered && m_checkboxHover < 1.0f) {
        m_checkboxHover = 1.0f;
        m_lastHoverTime = GetTickCount();
    } else if (!isCheckboxHovered && m_checkboxHover > 0.0f) {
        m_checkboxHover = 0.0f;
        m_lastHoverTime = GetTickCount();
    }
    
    // Pop checkbox styles if they were applied
    if (appliedCheckboxStyle) {
        ImGui::PopStyleColor(3); // Pop FrameBg, FrameBgHovered, CheckMark
    }
    

    
    ImGui::Spacing();
    
    // Status display
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
    ImGui::Text("%s: %zu", strings.robloxWindow.c_str(), m_rbxWindows.size());
    ImGui::PopStyleColor();
    
    // Count blocking windows
    int blockingCount = 0;
    for (const auto& window : m_rbxWindows) {
        if (window.hasSingletonEvent) blockingCount++;
    }
    
    // Update blocking state for animation
    bool currentBlockingState = (blockingCount > 0);
    if (currentBlockingState != m_lastBlockingState) {
        m_lastBlockingState = currentBlockingState;
        m_lastStatusChange = GetTickCount(); // Use fresh time for state change
    }
    

    
    if (blockingCount > 0) {
        // Animate from green to red
        float red = lerp(0.3f, 1.0f, m_statusColorLerp);
        float green = lerp(1.0f, 0.3f, m_statusColorLerp);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(red, green, 0.3f, 1.0f));
        if (m_fontAwesome) {
            ImGui::PushFont(m_fontAwesome);
            ImGui::Text("\xef\x83\x9d"); // fa-lock icon
            ImGui::PopFont();
            ImGui::SameLine();
        }
        ImGui::Text("%s: %d", strings.blocking.c_str(), blockingCount);
        ImGui::PopStyleColor();
    } else if (!m_rbxWindows.empty()) {
        // Animate from red to green
        float red = lerp(1.0f, 0.3f, m_statusColorLerp);
        float green = lerp(0.3f, 1.0f, m_statusColorLerp);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(red, green, 0.3f, 1.0f));
        if (m_fontAwesome) {
            ImGui::PushFont(m_fontAwesome);
            ImGui::Text("\xef\x82\x9e"); // fa-unlock icon
            ImGui::PopFont();
            ImGui::SameLine();
        }
        ImGui::Text("%s", strings.ok.c_str());
        ImGui::PopStyleColor();
    }
    

    
    ImGui::Unindent(16.0f); // Remove left padding
    // Bottom padding is automatically handled by centering calculation
    
    ImGui::EndChild();
    ImGui::PopStyleVar(); // Pop ChildBorderSize
    ImGui::PopStyleColor(); // Pop ChildBg
    

    
    ImGui::PopStyleVar(); // WindowPadding
    ImGui::End();
}

void UIManager::updateRbxWindows() {
    m_rbxWindows.clear();
    
    for (const auto& process : m_processes) {
        RbxWindow window;
        window.processId = process.processId;
        window.title = getWindowTitle(process.processId);
        window.handles = process.handles;
        window.hasSingletonEvent = !process.handles.empty();
        m_rbxWindows.push_back(window);
    }
}

std::string UIManager::getWindowTitle(DWORD processId) {
    // Simple approach - just return a descriptive name for now
    // This avoids potential crashes with complex window enumeration
    return "RBX Window";
}

bool UIManager::CreateDeviceD3D(HWND hWnd) {
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &featureLevel, &m_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &featureLevel, &m_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void UIManager::CleanupDeviceD3D() {
    CleanupRenderTarget();
    if (m_pSwapChain) { m_pSwapChain->Release(); m_pSwapChain = nullptr; }
    if (m_pd3dDeviceContext) { m_pd3dDeviceContext->Release(); m_pd3dDeviceContext = nullptr; }
    if (m_pd3dDevice) { m_pd3dDevice->Release(); m_pd3dDevice = nullptr; }
}

void UIManager::CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_mainRenderTargetView);
    pBackBuffer->Release();
}

void UIManager::CleanupRenderTarget() {
    if (m_mainRenderTargetView) { m_mainRenderTargetView->Release(); m_mainRenderTargetView = nullptr; }
}
