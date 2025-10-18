#include "imgui_impl_qt.h"
#include <cstdint>
#include <QElapsedTimer>
#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QInputMethod>
#include <QApplication> // Adicione esta linha no topo do arquivo, junto com outros includes
#include <iostream>

struct ImGui_ImplQt_Data
{
    QApplication*               QApp;
    QWidget*                    Widget;
    int                         MouseTrackedArea;   // 0: not tracked, 1: client area, 2: non-client area
    int                         MouseButtonsDown;
    QElapsedTimer               Timer;
    qint64                      LastTime = 0;
    double                      TicksPerSecond = 1e9; // QElapsedTimer uses nanoseconds
    ImGuiMouseCursor            LastMouseCursor;
    uint32_t                    KeyboardCodePage;

    char                        BackendPlatformName[48];

    ImGui_ImplQt_Data() { memset((void*)this, 0, sizeof(*this)); }
};

void ImGui_ImplQt_UpdateModifiers(Qt::KeyboardModifiers mods)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEvent(ImGuiKey_ModCtrl, mods & Qt::ControlModifier);
    io.AddKeyEvent(ImGuiKey_ModShift, mods & Qt::ShiftModifier);
    io.AddKeyEvent(ImGuiKey_ModAlt, mods & Qt::AltModifier);
    io.AddKeyEvent(ImGuiKey_ModSuper, mods & Qt::MetaModifier);
}

ImGuiKey ImGui_ImplQt_KeyEventToImGuiKey(int key)
{
    switch (key)
    {
        // --- Letters ---
    case Qt::Key_A: return ImGuiKey_A;
    case Qt::Key_B: return ImGuiKey_B;
    case Qt::Key_C: return ImGuiKey_C;
    case Qt::Key_D: return ImGuiKey_D;
    case Qt::Key_E: return ImGuiKey_E;
    case Qt::Key_F: return ImGuiKey_F;
    case Qt::Key_G: return ImGuiKey_G;
    case Qt::Key_H: return ImGuiKey_H;
    case Qt::Key_I: return ImGuiKey_I;
    case Qt::Key_J: return ImGuiKey_J;
    case Qt::Key_K: return ImGuiKey_K;
    case Qt::Key_L: return ImGuiKey_L;
    case Qt::Key_M: return ImGuiKey_M;
    case Qt::Key_N: return ImGuiKey_N;
    case Qt::Key_O: return ImGuiKey_O;
    case Qt::Key_P: return ImGuiKey_P;
    case Qt::Key_Q: return ImGuiKey_Q;
    case Qt::Key_R: return ImGuiKey_R;
    case Qt::Key_S: return ImGuiKey_S;
    case Qt::Key_T: return ImGuiKey_T;
    case Qt::Key_U: return ImGuiKey_U;
    case Qt::Key_V: return ImGuiKey_V;
    case Qt::Key_W: return ImGuiKey_W;
    case Qt::Key_X: return ImGuiKey_X;
    case Qt::Key_Y: return ImGuiKey_Y;
    case Qt::Key_Z: return ImGuiKey_Z;

        // --- Numbers ---
    case Qt::Key_0: return ImGuiKey_0;
    case Qt::Key_1: return ImGuiKey_1;
    case Qt::Key_2: return ImGuiKey_2;
    case Qt::Key_3: return ImGuiKey_3;
    case Qt::Key_4: return ImGuiKey_4;
    case Qt::Key_5: return ImGuiKey_5;
    case Qt::Key_6: return ImGuiKey_6;
    case Qt::Key_7: return ImGuiKey_7;
    case Qt::Key_8: return ImGuiKey_8;
    case Qt::Key_9: return ImGuiKey_9;

        // --- Function keys ---
    case Qt::Key_F1:  return ImGuiKey_F1;
    case Qt::Key_F2:  return ImGuiKey_F2;
    case Qt::Key_F3:  return ImGuiKey_F3;
    case Qt::Key_F4:  return ImGuiKey_F4;
    case Qt::Key_F5:  return ImGuiKey_F5;
    case Qt::Key_F6:  return ImGuiKey_F6;
    case Qt::Key_F7:  return ImGuiKey_F7;
    case Qt::Key_F8:  return ImGuiKey_F8;
    case Qt::Key_F9:  return ImGuiKey_F9;
    case Qt::Key_F10: return ImGuiKey_F10;
    case Qt::Key_F11: return ImGuiKey_F11;
    case Qt::Key_F12: return ImGuiKey_F12;

        // --- Modifiers ---
    case Qt::Key_Shift:   return ImGuiKey_LeftShift;
    case Qt::Key_Control: return ImGuiKey_LeftCtrl;
    case Qt::Key_Alt:     return ImGuiKey_LeftAlt;
    case Qt::Key_Meta:    return ImGuiKey_LeftSuper;

        // --- Arrows and navigation ---
    case Qt::Key_Left:   return ImGuiKey_LeftArrow;
    case Qt::Key_Right:  return ImGuiKey_RightArrow;
    case Qt::Key_Up:     return ImGuiKey_UpArrow;
    case Qt::Key_Down:   return ImGuiKey_DownArrow;
    case Qt::Key_PageUp:   return ImGuiKey_PageUp;
    case Qt::Key_PageDown: return ImGuiKey_PageDown;
    case Qt::Key_Home:     return ImGuiKey_Home;
    case Qt::Key_End:      return ImGuiKey_End;
    case Qt::Key_Insert:   return ImGuiKey_Insert;
    case Qt::Key_Delete:   return ImGuiKey_Delete;
    case Qt::Key_Backspace:return ImGuiKey_Backspace;

        // --- Editing ---
    case Qt::Key_Space:   return ImGuiKey_Space;
    case Qt::Key_Return:  return ImGuiKey_Enter;
    case Qt::Key_Enter:   return ImGuiKey_KeypadEnter;
    case Qt::Key_Tab:     return ImGuiKey_Tab;
    case Qt::Key_Escape:  return ImGuiKey_Escape;
    case Qt::Key_Comma:   return ImGuiKey_Comma;
    case Qt::Key_Period:  return ImGuiKey_Period;
    case Qt::Key_Slash:   return ImGuiKey_Slash;
    case Qt::Key_Backslash: return ImGuiKey_Backslash;
    case Qt::Key_Semicolon: return ImGuiKey_Semicolon;
    case Qt::Key_Apostrophe: return ImGuiKey_Apostrophe;
    case Qt::Key_Minus:   return ImGuiKey_Minus;
    case Qt::Key_Equal:   return ImGuiKey_Equal;
    case Qt::Key_BracketLeft:  return ImGuiKey_LeftBracket;
    case Qt::Key_BracketRight: return ImGuiKey_RightBracket;
    case Qt::Key_QuoteLeft:    return ImGuiKey_GraveAccent;

        // --- Keypad ---
    /*case Qt::Key_Plus:       return ImGuiKey_KeypadAdd;
    case Qt::Key_Minus:      return ImGuiKey_KeypadSubtract;
    case Qt::Key_Asterisk:   return ImGuiKey_KeypadMultiply;
    case Qt::Key_Slash:      return ImGuiKey_KeypadDivide;
    case Qt::Key_Enter:      return ImGuiKey_KeypadEnter;
    case Qt::Key_Period:     return ImGuiKey_KeypadDecimal;*/

    default: return ImGuiKey_None;
    }
}

static ImGui_ImplQt_Data* ImGui_ImplQt_GetBackendData()
{
    return ImGui::GetCurrentContext() ? (ImGui_ImplQt_Data*)ImGui::GetIO().BackendPlatformUserData : nullptr;
}
static ImGui_ImplQt_Data* ImGui_ImplQt_GetBackendData(ImGuiIO& io)
{
    return (ImGui_ImplQt_Data*)io.BackendPlatformUserData;
}

// Functions
static void ImGui_ImplQt_UpdateKeyboardCodePage(ImGuiIO& io)
{
    // Retrieve keyboard code page, required for handling of non-Unicode Windows.
    ImGui_ImplQt_Data* bd = ImGui_ImplQt_GetBackendData(io);
    auto currentInputLocale = bd->QApp->inputMethod();
    //if (::GetLocaleInfoA(keyboard_lcid, (LOCALE_RETURN_NUMBER | LOCALE_IDEFAULTANSICODEPAGE), (LPSTR)&bd->KeyboardCodePage, sizeof(bd->KeyboardCodePage)) == 0)
    //    bd->KeyboardCodePage = CP_ACP; // Fallback to default ANSI code page when fails.
}

bool ImGui_ImplQt_Init(QWidget* widget, QApplication* app)
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

    bd->QApp = app;
    bd->Widget = widget;
    bd->Timer.start();
    bd->LastTime = bd->Timer.nsecsElapsed();
    bd->TicksPerSecond = 1e9; // nanoseconds per second
    bd->LastMouseCursor = ImGuiMouseCursor_COUNT;
    ImGui_ImplQt_UpdateKeyboardCodePage(io);

    return true;
}

void ImGui_ImplQt_Shutdown()
{
    ImGui_ImplQt_Data* bd = ImGui_ImplQt_GetBackendData();
    IM_ASSERT(bd != nullptr && "No platform backend to shutdown, or already shutdown?");
    ImGuiIO& io = ImGui::GetIO();
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();

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

    qint64 currentTime = bd->Timer.nsecsElapsed();
    io.DeltaTime = float(currentTime - bd->LastTime) / float(bd->TicksPerSecond);
    bd->LastTime = currentTime;
}

void ImGui_ImplQt_ProcessEvent(QEvent* e)
{
    if (ImGui::GetCurrentContext() == nullptr)
        return;
    ImGuiIO& io = ImGui::GetIO();

    switch (e->type()) {
    case QEvent::MouseMove: {
        QMouseEvent* ev = reinterpret_cast<QMouseEvent*>(e);
        io.AddMousePosEvent(ev->position().x(), ev->position().y());
        break;
    }
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease: {
        QMouseEvent* ev = reinterpret_cast<QMouseEvent*>(e);
        int button = 0;
        if (ev->button() == Qt::LeftButton) button = 0;
        else if (ev->button() == Qt::RightButton) button = 1;
        else if (ev->button() == Qt::MiddleButton) button = 2;
        else if (ev->button() == Qt::XButton1) button = 3;
        else if (ev->button() == Qt::XButton2) button = 4;
        io.AddMouseButtonEvent(button, e->type() == QEvent::MouseButtonPress);
        break;
    }
    case QEvent::Wheel: {
        QWheelEvent* ev = reinterpret_cast<QWheelEvent*>(e);
        io.AddMouseWheelEvent(ev->angleDelta().x() / 120.0f, ev->angleDelta().y() / 120.0f);
        break;
    }
    case QEvent::KeyPress:
    case QEvent::KeyRelease: {
        QKeyEvent* ev = reinterpret_cast<QKeyEvent*>(e);
        ImGui_ImplQt_UpdateModifiers(ev->modifiers());

        ImGuiKey key = ImGui_ImplQt_KeyEventToImGuiKey(ev->key());
        if (key != ImGuiKey_None)
            io.AddKeyEvent(key, e->type() == QEvent::KeyPress);
        
        if (e->type() == QEvent::KeyPress)
        {
            QString text = ev->text();
            if (!text.isEmpty())
            {
                ImGuiIO& io = ImGui::GetIO();
                for (QChar ch : text) {
                    io.AddInputCharacter((ImWchar)ch.unicode());
                }
            }
        }
        break;
    }
    case QEvent::InputMethod: {
        QInputMethodEvent* ev = reinterpret_cast<QInputMethodEvent*>(e);
        io.AddInputCharactersUTF8(ev->commitString().toUtf8().constData());
        break;
    }
    case QEvent::FocusIn:
        io.AddFocusEvent(true);
        break;
    case QEvent::FocusOut:
        io.AddFocusEvent(false);
        break;
    default:
        break;
    }
}

