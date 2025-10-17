#include "imgui_impl_qt.h"
#include <cstdint>

struct ImGui_ImplQt_Data
{
    QImGuiWidget*               Widget;
    int                         MouseTrackedArea;   // 0: not tracked, 1: client area, 2: non-client area
    int                         MouseButtonsDown;
    int64_t                     Time;
    int64_t                     TicksPerSecond;
    ImGuiMouseCursor            LastMouseCursor;
    uint32_t                    KeyboardCodePage;

    char                        BackendPlatformName[48];

    ImGui_ImplQt_Data() { memset((void*)this, 0, sizeof(*this)); }
};


static ImGui_ImplQt_Data* ImGui_ImplQt_GetBackendData()
{
    return ImGui::GetCurrentContext() ? (ImGui_ImplQt_Data*)ImGui::GetIO().BackendPlatformUserData : nullptr;
}
static ImGui_ImplQt_Data* ImGui_ImplQt_GetBackendData(ImGuiIO& io)
{
    return (ImGui_ImplQt_Data*)io.BackendPlatformUserData;
}

// Functions
//static void ImGui_ImplQt_UpdateKeyboardCodePage(ImGuiIO& io)
//{
//    // Retrieve keyboard code page, required for handling of non-Unicode Windows.
//    ImGui_ImplQt_Data* bd = ImGui_ImplWin32_GetBackendData(io);
//    HKL keyboard_layout = ::GetKeyboardLayout(0);
//    LCID keyboard_lcid = MAKELCID(HIWORD(keyboard_layout), SORT_DEFAULT);
//    if (::GetLocaleInfoA(keyboard_lcid, (LOCALE_RETURN_NUMBER | LOCALE_IDEFAULTANSICODEPAGE), (LPSTR)&bd->KeyboardCodePage, sizeof(bd->KeyboardCodePage)) == 0)
//        bd->KeyboardCodePage = CP_ACP; // Fallback to default ANSI code page when fails.
//}

bool ImGui_ImplQt_Init(QImGuiWidget* widget)
{
    ImGuiIO& io = ImGui::GetIO();
    IMGUI_CHECKVERSION();
    IM_ASSERT(io.BackendPlatformUserData == nullptr && "Already initialized a platform backend!");

    // Setup backend capabilities flags
    ImGui_ImplQt_Data* bd = IM_NEW(ImGui_ImplQt_Data)();
    io.BackendPlatformUserData = (void*)bd;
    snprintf(bd->BackendPlatformName, sizeof(bd->BackendPlatformName), "imgui_impl_qt (%d.%d.%d)",
        QT_VERSION_MAJOR, QT_VERSION_MINOR, QT_VERSION_PATCH);
    io.BackendPlatformUserData = (void*)bd;
    io.BackendPlatformName = bd->BackendPlatformName;
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)

    bd->Widget = widget;
    bd->TicksPerSecond = 0;
    bd->Time = 0;
    bd->LastMouseCursor = ImGuiMouseCursor_COUNT;
    //ImGui_ImplQt_UpdateKeyboardCodePage(io);

    return true;
}

void ImGui_ImplQt_Shutdown()
{
    ImGui_ImplQt_Data* bd = ImGui_ImplQt_GetBackendData();
    IM_ASSERT(bd != nullptr && "No platform backend to shutdown, or already shutdown?");
    ImGuiIO& io = ImGui::GetIO();
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();

    // Unload XInput library

    io.BackendPlatformName = nullptr;
    io.BackendPlatformUserData = nullptr;
    io.BackendFlags &= ~(ImGuiBackendFlags_HasMouseCursors | ImGuiBackendFlags_HasSetMousePos | ImGuiBackendFlags_HasGamepad);
    platform_io.ClearPlatformHandlers();
    IM_DELETE(bd); 
}

void ImGui_ImplQt_NewFrame()
{
    ImGui_ImplQt_Data* bd = ImGui_ImplQt_GetBackendData();
    IM_ASSERT(bd != nullptr && "Context or backend not initialized? Did you call ImGui_ImplQt_Init()?");
    ImGuiIO& io = ImGui::GetIO();

    // Setup display size (every frame to accommodate for window resizing)
    QRect rect = bd->Widget->geometry();
    io.DisplaySize = ImVec2((float)(rect.right() - rect.left()), (float)(rect.bottom() - rect.top()));
}

void ImGui_ImplQt_EnableDpiAwareness()
{
    return;
}

float ImGui_ImplQt_GetDpiScaleForHwnd(void* hwnd)
{
    return 0.0f;
}

float ImGui_ImplQt_GetDpiScaleForMonitor(void* monitor)
{
    return 0.0f;
}

void ImGui_ImplQt_EnableAlphaCompositing(void* hwnd)
{
    return;
}
