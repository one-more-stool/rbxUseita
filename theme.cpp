#include "theme.h"
#include "imgui.h"

// Helper function for linear interpolation
static float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

void Theme::ApplyDarkTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    
    // Apply color scheme
    colors[ImGuiCol_Text] = ImVec4(Colors::Text[0], Colors::Text[1], Colors::Text[2], Colors::Text[3]);
    colors[ImGuiCol_TextDisabled] = ImVec4(Colors::TextDisabled[0], Colors::TextDisabled[1], Colors::TextDisabled[2], Colors::TextDisabled[3]);
    colors[ImGuiCol_WindowBg] = ImVec4(Colors::WindowBg[0], Colors::WindowBg[1], Colors::WindowBg[2], Colors::WindowBg[3]);
    colors[ImGuiCol_ChildBg] = ImVec4(Colors::ChildBg[0], Colors::ChildBg[1], Colors::ChildBg[2], Colors::ChildBg[3]);
    colors[ImGuiCol_PopupBg] = ImVec4(Colors::PopupBg[0], Colors::PopupBg[1], Colors::PopupBg[2], Colors::PopupBg[3]);
    colors[ImGuiCol_Border] = ImVec4(Colors::Border[0], Colors::Border[1], Colors::Border[2], Colors::Border[3]);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(Colors::FrameBg[0], Colors::FrameBg[1], Colors::FrameBg[2], Colors::FrameBg[3]);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(Colors::FrameBgHovered[0], Colors::FrameBgHovered[1], Colors::FrameBgHovered[2], Colors::FrameBgHovered[3]);
    colors[ImGuiCol_FrameBgActive] = ImVec4(Colors::FrameBgActive[0], Colors::FrameBgActive[1], Colors::FrameBgActive[2], Colors::FrameBgActive[3]);
    colors[ImGuiCol_TitleBg] = ImVec4(Colors::TitleBg[0], Colors::TitleBg[1], Colors::TitleBg[2], Colors::TitleBg[3]);
    colors[ImGuiCol_TitleBgActive] = ImVec4(Colors::TitleBgActive[0], Colors::TitleBgActive[1], Colors::TitleBgActive[2], Colors::TitleBgActive[3]);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(Colors::TitleBg[0], Colors::TitleBg[1], Colors::TitleBg[2], 0.75f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(Colors::FrameBg[0], Colors::FrameBg[1], Colors::FrameBg[2], Colors::FrameBg[3]);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(Colors::FrameBg[0], Colors::FrameBg[1], Colors::FrameBg[2], 0.60f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(Colors::Button[0], Colors::Button[1], Colors::Button[2], Colors::Button[3]);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(Colors::ButtonHovered[0], Colors::ButtonHovered[1], Colors::ButtonHovered[2], Colors::ButtonHovered[3]);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(Colors::ButtonActive[0], Colors::ButtonActive[1], Colors::ButtonActive[2], Colors::ButtonActive[3]);
    colors[ImGuiCol_CheckMark] = ImVec4(Colors::CheckMark[0], Colors::CheckMark[1], Colors::CheckMark[2], Colors::CheckMark[3]);
    colors[ImGuiCol_SliderGrab] = ImVec4(Colors::Button[0], Colors::Button[1], Colors::Button[2], Colors::Button[3]);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(Colors::ButtonActive[0], Colors::ButtonActive[1], Colors::ButtonActive[2], Colors::ButtonActive[3]);
    colors[ImGuiCol_Button] = ImVec4(Colors::Button[0], Colors::Button[1], Colors::Button[2], Colors::Button[3]);
    colors[ImGuiCol_ButtonHovered] = ImVec4(Colors::ButtonHovered[0], Colors::ButtonHovered[1], Colors::ButtonHovered[2], Colors::ButtonHovered[3]);
    colors[ImGuiCol_ButtonActive] = ImVec4(Colors::ButtonActive[0], Colors::ButtonActive[1], Colors::ButtonActive[2], Colors::ButtonActive[3]);
    colors[ImGuiCol_Header] = ImVec4(Colors::Button[0], Colors::Button[1], Colors::Button[2], 0.45f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(Colors::ButtonHovered[0], Colors::ButtonHovered[1], Colors::ButtonHovered[2], 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(Colors::ButtonActive[0], Colors::ButtonActive[1], Colors::ButtonActive[2], Colors::ButtonActive[3]);
    colors[ImGuiCol_Separator] = ImVec4(Colors::Separator[0], Colors::Separator[1], Colors::Separator[2], Colors::Separator[3]);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(Colors::ButtonHovered[0], Colors::ButtonHovered[1], Colors::ButtonHovered[2], Colors::ButtonHovered[3]);
    colors[ImGuiCol_SeparatorActive] = ImVec4(Colors::ButtonActive[0], Colors::ButtonActive[1], Colors::ButtonActive[2], Colors::ButtonActive[3]);
    colors[ImGuiCol_ResizeGrip] = ImVec4(Colors::Button[0], Colors::Button[1], Colors::Button[2], 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(Colors::ButtonHovered[0], Colors::ButtonHovered[1], Colors::ButtonHovered[2], 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(Colors::ButtonActive[0], Colors::ButtonActive[1], Colors::ButtonActive[2], 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(Colors::Button[0], Colors::Button[1], Colors::Button[2], 0.73f);
    colors[ImGuiCol_TabHovered] = ImVec4(Colors::ButtonHovered[0], Colors::ButtonHovered[1], Colors::ButtonHovered[2], 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(Colors::ButtonActive[0], Colors::ButtonActive[1], Colors::ButtonActive[2], Colors::ButtonActive[3]);
    colors[ImGuiCol_TabUnfocused] = ImVec4(Colors::Button[0], Colors::Button[1], Colors::Button[2], 0.39f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(Colors::ButtonHovered[0], Colors::ButtonHovered[1], Colors::ButtonHovered[2], 0.67f);
    colors[ImGuiCol_PlotLines] = ImVec4(Colors::Text[0], Colors::Text[1], Colors::Text[2], Colors::Text[3]);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(Colors::TextWarning[0], Colors::TextWarning[1], Colors::TextWarning[2], Colors::TextWarning[3]);
    colors[ImGuiCol_PlotHistogram] = ImVec4(Colors::TextWarning[0], Colors::TextWarning[1], Colors::TextWarning[2], Colors::TextWarning[3]);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(Colors::TextSuccess[0], Colors::TextSuccess[1], Colors::TextSuccess[2], Colors::TextSuccess[3]);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(Colors::ButtonActive[0], Colors::ButtonActive[1], Colors::ButtonActive[2], 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(Colors::CheckMark[0], Colors::CheckMark[1], Colors::CheckMark[2], 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(Colors::ButtonHovered[0], Colors::ButtonHovered[1], Colors::ButtonHovered[2], Colors::ButtonHovered[3]);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
    
    // Apply style settings
    style.WindowRounding = Style::WindowRounding;
    style.FrameRounding = Style::FrameRounding;
    style.GrabRounding = Style::GrabRounding;
    style.WindowPadding = ImVec2(Style::WindowPadding[0], Style::WindowPadding[1]);
    style.FramePadding = ImVec2(Style::FramePadding[0], Style::FramePadding[1]);
    style.ItemSpacing = ImVec2(Style::ItemSpacing[0], Style::ItemSpacing[1]);
    style.ItemInnerSpacing = ImVec2(Style::ItemInnerSpacing[0], Style::ItemInnerSpacing[1]);
    style.IndentSpacing = Style::IndentSpacing;
    style.ScrollbarSize = Style::ScrollbarSize;
    style.ScrollbarRounding = Style::ScrollbarRounding;
    style.GrabMinSize = Style::GrabMinSize;
    style.WindowBorderSize = Style::BorderSize;
    style.ChildBorderSize = Style::BorderSize;
    style.PopupBorderSize = Style::BorderSize;
    style.FrameBorderSize = 0.0f;
    style.TabBorderSize = 0.0f;
}

void Theme::ApplyCustomButtonStyle(bool isExitButton) {
    ImGuiStyle& style = ImGui::GetStyle();
    
    if (isExitButton) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(Colors::ExitButton[0], Colors::ExitButton[1], Colors::ExitButton[2], Colors::ExitButton[3]));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(Colors::ExitButtonHovered[0], Colors::ExitButtonHovered[1], Colors::ExitButtonHovered[2], Colors::ExitButtonHovered[3]));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(Colors::ExitButtonActive[0], Colors::ExitButtonActive[1], Colors::ExitButtonActive[2], Colors::ExitButtonActive[3]));
    } else {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(Colors::Button[0], Colors::Button[1], Colors::Button[2], Colors::Button[3]));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(Colors::ButtonHovered[0], Colors::ButtonHovered[1], Colors::ButtonHovered[2], Colors::ButtonHovered[3]));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(Colors::ButtonActive[0], Colors::ButtonActive[1], Colors::ButtonActive[2], Colors::ButtonActive[3]));
    }
}

void Theme::ApplyAnimatedExitButtonStyle(float hoverProgress) {
    // Interpolate exit button colors based on hover progress
    float baseR = Colors::ExitButton[0];
    float baseG = Colors::ExitButton[1]; 
    float baseB = Colors::ExitButton[2];
    
    float hoverR = Colors::ExitButtonHovered[0];
    float hoverG = Colors::ExitButtonHovered[1];
    float hoverB = Colors::ExitButtonHovered[2];
    
    // Linear interpolation between base and hover colors
    float animR = baseR + (hoverR - baseR) * hoverProgress;
    float animG = baseG + (hoverG - baseG) * hoverProgress;
    float animB = baseB + (hoverB - baseB) * hoverProgress;
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(animR, animG, animB, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(animR, animG, animB, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(Colors::ExitButtonActive[0], Colors::ExitButtonActive[1], Colors::ExitButtonActive[2], Colors::ExitButtonActive[3]));
}

void Theme::ApplyAnimatedCheckboxStyle(float hoverProgress) {
    // Interpolate checkbox colors based on hover progress
    float baseR = 0.18f, baseG = 0.18f, baseB = 0.22f; // Base FrameBg color
    float hoverR = 0.25f, hoverG = 0.25f, hoverB = 0.30f; // Hover FrameBg color
    
    float animR = lerp(baseR, hoverR, hoverProgress);
    float animG = lerp(baseG, hoverG, hoverProgress);
    float animB = lerp(baseB, hoverB, hoverProgress);
    
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(animR, animG, animB, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(animR + 0.05f, animG + 0.05f, animB + 0.05f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(Colors::CheckMark[0], Colors::CheckMark[1], Colors::CheckMark[2], Colors::CheckMark[3]));
}

void Theme::ResetButtonStyle() {
    ImGui::PopStyleColor(3); // Pop Button, ButtonHovered, ButtonActive
}
