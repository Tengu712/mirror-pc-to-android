#include "window.h"

#include <dwmapi.h>

typedef struct Param_t {
    DWORD pid;
    HWND window;
} Param;

BOOL CALLBACK callback(HWND window, LPARAM lp) {
    Param *param = (Param *)lp;

    // skip if PID is different
    DWORD pid;
    GetWindowThreadProcessId(window, &pid);
    if (pid != param->pid) {
        return TRUE;
    }

    // skip if it's invisible
    if (!IsWindowVisible(window)) {
        return TRUE;
    }

    // skip if it's a child window
    DWORD styles = (DWORD)GetWindowLongPtrA(window, GWL_STYLE);
    if (styles & WS_CHILD) {
        return TRUE;
    }

    // skip if it's a cloaked window
    DWORD cloaked;
    HRESULT res = DwmGetWindowAttribute(window, DWMWA_CLOAKED, &cloaked, sizeof(DWORD));
    if (res != S_OK || cloaked) {
        return TRUE;
    }

    // it is
    param->window = window;
    return FALSE;
}

HWND getWindowHandleFromProcessID(DWORD pid) {
    Param param;
    param.pid = pid;
    param.window = NULL;
    EnumWindows(callback, (LPARAM)&param);
    return param.window;
}
