#pragma once

namespace Theme {
    // Color definitions
    struct Colors {
        // Background colors
        static constexpr float WindowBg[4] = { 0.08f, 0.08f, 0.10f, 1.0f };
        static constexpr float ChildBg[4] = { 0.12f, 0.12f, 0.15f, 1.0f };
        static constexpr float PopupBg[4] = { 0.15f, 0.15f, 0.18f, 1.0f };
        
        // Title bar colors
        static constexpr float TitleBg[4] = { 0.05f, 0.05f, 0.07f, 1.0f };
        static constexpr float TitleBgActive[4] = { 0.08f, 0.08f, 0.10f, 1.0f };
        
        // Button colors
        static constexpr float Button[4] = { 0.20f, 0.25f, 0.35f, 1.0f };
        static constexpr float ButtonHovered[4] = { 0.30f, 0.35f, 0.45f, 1.0f };
        static constexpr float ButtonActive[4] = { 0.35f, 0.40f, 0.50f, 1.0f };
        
        // Exit button colors (red theme)
        static constexpr float ExitButton[4] = { 0.60f, 0.15f, 0.15f, 1.0f };
        static constexpr float ExitButtonHovered[4] = { 0.80f, 0.20f, 0.20f, 1.0f };
        static constexpr float ExitButtonActive[4] = { 0.90f, 0.25f, 0.25f, 1.0f };
        
        // Text colors
        static constexpr float Text[4] = { 0.95f, 0.95f, 0.95f, 1.0f };
        static constexpr float TextDisabled[4] = { 0.50f, 0.50f, 0.50f, 1.0f };
        static constexpr float TextSuccess[4] = { 0.30f, 0.90f, 0.30f, 1.0f };
        static constexpr float TextWarning[4] = { 0.90f, 0.70f, 0.30f, 1.0f };
        static constexpr float TextError[4] = { 0.90f, 0.30f, 0.30f, 1.0f };
        
        // Frame colors
        static constexpr float FrameBg[4] = { 0.18f, 0.18f, 0.22f, 1.0f };
        static constexpr float FrameBgHovered[4] = { 0.25f, 0.25f, 0.30f, 1.0f };
        static constexpr float FrameBgActive[4] = { 0.30f, 0.30f, 0.35f, 1.0f };
        
        // Checkbox colors
        static constexpr float CheckMark[4] = { 0.30f, 0.90f, 0.30f, 1.0f };
        
        // Border colors
        static constexpr float Border[4] = { 0.30f, 0.30f, 0.35f, 1.0f };
        static constexpr float Separator[4] = { 0.25f, 0.25f, 0.30f, 1.0f };
    };
    
    // Style settings
    struct Style {
        static constexpr float WindowRounding = 8.0f;
        static constexpr float FrameRounding = 4.0f;
        static constexpr float GrabRounding = 4.0f;
        static constexpr float WindowPadding[2] = { 12.0f, 12.0f };
        static constexpr float FramePadding[2] = { 8.0f, 4.0f };
        static constexpr float ItemSpacing[2] = { 8.0f, 6.0f };
        static constexpr float ItemInnerSpacing[2] = { 6.0f, 4.0f };
        static constexpr float IndentSpacing = 20.0f;
        static constexpr float ScrollbarSize = 14.0f;
        static constexpr float ScrollbarRounding = 8.0f;
        static constexpr float GrabMinSize = 12.0f;
        static constexpr float BorderSize = 1.0f;
    };
    
    // Apply theme to ImGui
    void ApplyDarkTheme();
    void ApplyCustomButtonStyle(bool isExitButton = false);
    void ApplyAnimatedExitButtonStyle(float hoverProgress);
    void ApplyAnimatedCheckboxStyle(float hoverProgress);
    void ResetButtonStyle();
}
