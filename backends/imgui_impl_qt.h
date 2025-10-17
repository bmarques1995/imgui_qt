// dear imgui: Platform Backend for Qt (standard windows API for 32-bits AND 64-bits applications)
// This needs to be used along with a Renderer (e.g. DirectX11, OpenGL3, Vulkan..)

// Implemented features:
//  [ ] Platform: Clipboard support (for Win32 this is actually part of core dear imgui)
//  [ ] Platform: Mouse support. Can discriminate Mouse/TouchScreen/Pen.
//  [ ] Platform: Keyboard support. Since 1.87 we are using the io.AddKeyEvent() function. Pass ImGuiKey values to all key functions e.g. ImGui::IsKeyPressed(ImGuiKey_Space). [Legacy VK_* values are obsolete since 1.87 and not supported since 1.91.5]
//  [ ] Platform: Gamepad support.
//  [ ] Platform: Mouse cursor shape and visibility (ImGuiBackendFlags_HasMouseCursors). Disable with 'io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange'.

// You can use unmodified imgui_impl_* files in your project. See examples/ folder for examples of using this.
// Prefer including the entire imgui/ repository into your project (either as a copy or as a submodule), and only build the backends you need.
// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#pragma once
#include "imgui.h"      // IMGUI_IMPL_API
#include <QWidget>
#ifndef IMGUI_DISABLE

class QImGuiWidget : public QWidget
{
public:
    
};

// Follow "Getting Started" link and check examples/ folder to learn about using backends!
IMGUI_IMPL_API bool     ImGui_ImplQt_Init(QImGuiWidget* widget);
//IMGUI_IMPL_API bool     ImGui_ImplWin32_InitForOpenGL(void* hwnd);
IMGUI_IMPL_API void     ImGui_ImplQt_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplQt_NewFrame();

#endif // #ifndef IMGUI_DISABLE
