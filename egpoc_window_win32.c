#include "egpoc_window.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#include <stdio.h>

#include "egpoc_debug.h"
#include "egpoc_memory.h"
#include "egpoc_system.h"

typedef struct egpoc_window_t {
    HWND hwnd;
} egpoc_window_t;

static LRESULT CALLBACK egpoc_window_win32_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_CREATE:
        EGPOC_DEBUG("WM_CREATE\n");
        break;
    case WM_DESTROY:
        EGPOC_DEBUG("WM_DESTROY\n");
        PostQuitMessage(0);
        break;
    case WM_CLOSE:
        EGPOC_DEBUG("WM_CLOSE\n");
        DestroyWindow(hwnd);
        break;
    case WM_PAINT: {
        EGPOC_DEBUG("WM_PAINT\n");
        PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_MOVE:
        EGPOC_DEBUG("WM_MOVE: x=%d, y=%d\n", (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
        break;
    case WM_SIZE:
        EGPOC_DEBUG("WM_SIZE: width=%d, height=%d\n", LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_KEYDOWN:
        EGPOC_DEBUG("WM_KEYDOWN: key=%u\n", (unsigned int)wParam);
        break;
    case WM_KEYUP:
        EGPOC_DEBUG("WM_KEYUP: key=%u\n", (unsigned int)wParam);
        break;
    case WM_CHAR:
        EGPOC_DEBUG("WM_CHAR: char='%c'\n", (char)wParam);
        break;
    case WM_MOUSEMOVE:
        EGPOC_DEBUG("WM_MOUSEMOVE: x=%d, y=%d\n", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_LBUTTONDOWN:
        EGPOC_DEBUG("WM_LBUTTONDOWN: x=%d, y=%d\n", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_LBUTTONUP:
        EGPOC_DEBUG("WM_LBUTTONUP: x=%d, y=%d\n", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_RBUTTONDOWN:
        EGPOC_DEBUG("WM_RBUTTONDOWN: x=%d, y=%d\n", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_RBUTTONUP:
        EGPOC_DEBUG("WM_RBUTTONUP: x=%d, y=%d\n", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_MOUSEWHEEL:
        EGPOC_DEBUG("WM_MOUSEWHEEL: delta=%d\n", GET_WHEEL_DELTA_WPARAM(wParam));
        break;
    case WM_SETFOCUS:
        EGPOC_DEBUG("WM_SETFOCUS\n");
        break;
    case WM_KILLFOCUS:
        EGPOC_DEBUG("WM_KILLFOCUS\n");
        break;
    case WM_ACTIVATE:
        EGPOC_DEBUG("WM_ACTIVATE: state=%d\n", LOWORD(wParam));
        break;
    case WM_SHOWWINDOW:
        EGPOC_DEBUG("WM_SHOWWINDOW: shown=%d\n", (int)wParam);
        break;
    default:
        EGPOC_DEBUG(
            "Message: 0x%04X (wParam=0x%08X, lParam=0x%08X)\n", msg, (unsigned int)wParam, (unsigned int)lParam);
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

egpoc_system_error_t egpoc_window_create(egpoc_memory_owner_t*  memory_owner,
                                         egpoc_memory_acquire_t memory_acquire,
                                         egpoc_memory_release_t memory_release,
                                         char const*            window_title,
                                         unsigned int           window_width,
                                         unsigned int           window_height,
                                         egpoc_window_t**       platform)
{
    egpoc_window_t* platform_win32 = (egpoc_window_t*)memory_acquire(memory_owner, sizeof(egpoc_window_t));

    if (!platform_win32) {
        return egpoc_system_error_unknown;
    }

    WNDCLASS wc      = {0};
    wc.lpfnWndProc   = egpoc_window_win32_WndProc;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.lpszClassName = "egpoc";

    if (!RegisterClass(&wc)) {
        platform_win32 = memory_release(memory_owner, sizeof(egpoc_window_t), platform_win32);
        return egpoc_system_error_unknown;
    }

    HWND hwnd = CreateWindowEx(0,
                               wc.lpszClassName,
                               window_title,
                               WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                               CW_USEDEFAULT,
                               CW_USEDEFAULT,
                               (int)window_width,
                               (int)window_height,
                               NULL,
                               NULL,
                               wc.hInstance,
                               NULL);

    if (!hwnd) {
        platform_win32 = memory_release(memory_owner, sizeof(egpoc_window_t), platform_win32);
        return egpoc_system_error_unknown;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    platform_win32->hwnd = hwnd;

    // SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)platform);

    *platform = platform_win32;

    return egpoc_system_error_none;
}

egpoc_system_error_t egpoc_window_events(egpoc_window_t*       platform,
                                         egpoc_window_event_t* events,
                                         size_t                events_count_limit,
                                         size_t*               events_count)
{
    (void)events;

    egpoc_window_t* platform_win32 = (egpoc_window_t*)platform;

    HWND hwnd = platform_win32->hwnd;

    MSG msg;
    memset(&msg, 0, sizeof(msg));

    size_t events_count_ = 0;

    while (events_count_ < events_count_limit) {
        if (!PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
            break;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);

        // TODO: save event
        events_count_++;
    }

    *events_count = events_count_;

    return egpoc_system_error_none;
}
