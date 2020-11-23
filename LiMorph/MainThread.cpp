#include "pch.h"
#include "MainThread.h"

namespace morph {

WNDPROC m_old_callback;
_callback* m_invoke;

// Hook main window
LRESULT CALLBACK MainThread::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    // If message was not sent by us, pass message to old callback
    if (Msg != WM_USER && (wParam != UserMessage::Startup && wParam != UserMessage::Shutdown)) {
        return CallWindowProc(m_old_callback, hWnd, Msg, wParam, lParam);
    }

    // Otherwise do our stuff in main thread.
    m_invoke();
    return 0;
}

void MainThread::InvokeInMainThread(_callback* callback) {
    m_handle = find_main_window(GetCurrentProcessId()); // can we run this once? i.e., move to constructor?
    m_invoke = callback;
    m_new_callback = WndProc;
    Attach();
    Invoke(UserMessage::Startup);
    // Do something before detaching, not sure what yet.
    // Invoke(UserMessage::Shutdown);
    //Detach();

}

// Template WndProc method. Anything calling InvokeInMainThread must define their own 
// according to the below. (MAYBE)


void MainThread::Attach() {
    m_old_callback =
        (WNDPROC)SetWindowLongPtr(m_handle, GWL_WNDPROC, (uintptr_t)m_new_callback);
    if (m_old_callback == 0) {
        // do something
    }

}

void MainThread::Detach() {
    if (m_new_callback == nullptr || m_old_callback == nullptr)
        return;

    SetWindowLongPtr(m_handle, GWL_WNDPROC, (uintptr_t)m_old_callback);
    m_new_callback = nullptr;
}

void MainThread::Invoke(int message) {
    SendMessage(m_handle, WM_USER, message, 0);
}

HWND MainThread::find_main_window(unsigned long process_id)
{
    handle_data data;
    data.process_id = process_id;
    data.window_handle = 0;
    EnumWindows(MainThread::enum_windows_callback, (LPARAM)&data);
    return data.window_handle;
}


BOOL CALLBACK MainThread::enum_windows_callback(HWND handle, LPARAM lParam) {
    handle_data& data = *(handle_data*)lParam;
    unsigned long process_id = 0;
    GetWindowThreadProcessId(handle, &process_id);
    if (data.process_id != process_id || !is_main_window(handle))
        return TRUE;
    data.window_handle = handle;
    return FALSE;
}

BOOL MainThread::is_main_window(HWND handle)
{
    return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}

} // namespace morph