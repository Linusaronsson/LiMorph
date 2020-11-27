#pragma once

namespace LiMorph {


typedef LRESULT WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
typedef void _callback();

constexpr int GWL_WNDPROC = -4;

enum UserMessage {
    Startup = 0x938983,
    Shutdown = 0x223278
};

struct handle_data {
    unsigned long process_id;
    HWND window_handle;
};

class MainThread
{
public:
    MainThread() {
        m_handle = find_main_window(GetCurrentProcessId()); // can we run this once? i.e., move to constructor?
    }
    ~MainThread() {}

    // Run startup() in main thread.
    void invokeInMainThread(_callback* startup);

    // find main window handle (could be private)
    HWND find_main_window(unsigned long process_id);

    // could be private if WndProc not static (maybe change at some point)

private:

    // Hook main window
    static LRESULT WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

    void Attach();
    void Detach();
    void Invoke(int message);
    bool IsUserMessage(int message);

    // Find main window handle
    static BOOL is_main_window(HWND handle);
    static BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lParam);

    WindowProc* m_new_callback;
    HWND m_handle;
    uintptr_t m_player;
};


} // namespace morph

