#ifdef EGPOC_PLATFORM_WIN32

#include "egpoc_platform.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#include <stdio.h>

#include "egpoc_memory.h"

typedef struct {
    HWND hwnd;
} egpoc_platform_win32_t;

static LRESULT CALLBACK egpoc_platform_win32_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_CREATE:
        fprintf(stderr, "WM_CREATE\n");
        break;
    case WM_DESTROY:
        fprintf(stderr, "WM_DESTROY\n");
        PostQuitMessage(0);
        break;
    case WM_CLOSE:
        fprintf(stderr, "WM_CLOSE\n");
        DestroyWindow(hwnd);
        break;
    case WM_PAINT: {
        fprintf(stderr, "WM_PAINT\n");
        PAINTSTRUCT ps;
        HDC         hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_MOVE:
        fprintf(stderr, "WM_MOVE: x=%d, y=%d\n", (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
        break;
    case WM_SIZE:
        fprintf(stderr, "WM_SIZE: width=%d, height=%d\n", LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_KEYDOWN:
        fprintf(stderr, "WM_KEYDOWN: key=%u\n", (unsigned int)wParam);
        break;
    case WM_KEYUP:
        fprintf(stderr, "WM_KEYUP: key=%u\n", (unsigned int)wParam);
        break;
    case WM_CHAR:
        fprintf(stderr, "WM_CHAR: char='%c'\n", (char)wParam);
        break;
    case WM_MOUSEMOVE:
        fprintf(stderr, "WM_MOUSEMOVE: x=%d, y=%d\n", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_LBUTTONDOWN:
        fprintf(stderr, "WM_LBUTTONDOWN: x=%d, y=%d\n", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_LBUTTONUP:
        fprintf(stderr, "WM_LBUTTONUP: x=%d, y=%d\n", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_RBUTTONDOWN:
        fprintf(stderr, "WM_RBUTTONDOWN: x=%d, y=%d\n", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_RBUTTONUP:
        fprintf(stderr, "WM_RBUTTONUP: x=%d, y=%d\n", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_MOUSEWHEEL:
        fprintf(stderr, "WM_MOUSEWHEEL: delta=%d\n", GET_WHEEL_DELTA_WPARAM(wParam));
        break;
    case WM_SETFOCUS:
        fprintf(stderr, "WM_SETFOCUS\n");
        break;
    case WM_KILLFOCUS:
        fprintf(stderr, "WM_KILLFOCUS\n");
        break;
    case WM_ACTIVATE:
        fprintf(stderr, "WM_ACTIVATE: state=%d\n", LOWORD(wParam));
        break;
    case WM_SHOWWINDOW:
        fprintf(stderr, "WM_SHOWWINDOW: shown=%d\n", (int)wParam);
        break;
    default:
        fprintf(stderr,
                "Message: 0x%04X (wParam=0x%08X, lParam=0x%08X)\n",
                msg,
                (unsigned int)wParam,
                (unsigned int)lParam);
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

egpoc_platform_error_t egpoc_platform_create(egpoc_memory_owner_t*  memory_owner,
                                             egpoc_memory_acquire_t memory_acquire,
                                             egpoc_memory_release_t memory_release,
                                             char const*            window_title,
                                             int                    window_width,
                                             int                    window_height,
                                             egpoc_platform_t**     platform)
{
    WNDCLASS wc      = {0};
    wc.lpfnWndProc   = egpoc_platform_win32_WndProc;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.lpszClassName = "egpoc";

    if (!RegisterClass(&wc)) {
        return egpoc_platform_error_unknown;
    }

    HWND hwnd = CreateWindowEx(0,
                               wc.lpszClassName,
                               window_title,
                               WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                               CW_USEDEFAULT,
                               CW_USEDEFAULT,
                               window_width,
                               window_width,
                               NULL,
                               NULL,
                               wc.hInstance,
                               NULL);

    if (!hwnd) {
        return egpoc_platform_error_unknown;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    egpoc_platform_win32_t* platform_win32
        = (egpoc_platform_win32_t*)memory_acquire(memory_owner, sizeof(egpoc_platform_win32_t));

    if (!platform_win32) {
        return egpoc_platform_error_unknown;
    }

    platform_win32->hwnd = hwnd;

    // SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)platform);

    *platform = platform_win32;

    return egpoc_platform_error_none;
}

egpoc_platform_error_t egpoc_platform_events(egpoc_platform_t*       platform,
                                             egpoc_platform_event_t* events,
                                             int                     events_count_limit,
                                             int*                    events_count)
{
    egpoc_platform_win32_t* platform_x11 = (egpoc_platform_win32_t*)platform;

    HWND hwnd = ((egpoc_platform_win32_t*)platform)->hwnd;

    MSG msg = {};

    int events_count_ = 0;

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

    return egpoc_platform_error_none;
}

#endif  // EGPOC_PLATFORM_WIN32
