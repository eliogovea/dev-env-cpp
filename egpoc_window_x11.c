#include "egpoc_window.h"

#define _POSIX_C_SOURCE 200809L
#include <unistd.h>

#include <stdio.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "egpoc_debug.h"

typedef struct egpoc_window_t {
    Display* display;
    Window   window;
} egpoc_window_t;

egpoc_system_error_t egpoc_window_create(egpoc_memory_owner_t*  memory_owner,
                                         egpoc_memory_acquire_t memory_acquire,
                                         egpoc_memory_release_t memory_release,
                                         char const*            window_title,
                                         unsigned int           window_width,
                                         unsigned int           window_height,
                                         egpoc_window_t**       platform)
{
    Display* display;
    int      screen;
    Window   window;

    egpoc_window_t* platform_x11 = (egpoc_window_t*)memory_acquire(memory_owner, sizeof(egpoc_window_t));

    if (!platform_x11) {
        platform_x11 = memory_release(memory_owner, sizeof(egpoc_window_t), platform_x11);
        return egpoc_system_error_unknown;
    }

    display = XOpenDisplay(NULL);

    if (!display) {
        platform_x11 = memory_release(memory_owner, sizeof(egpoc_window_t), platform_x11);
        return egpoc_system_error_unknown;
    }

    screen = DefaultScreen(display);

    Window root = RootWindow(display, screen);

    XSetWindowAttributes windowAttributes;
    memset(&windowAttributes, 0, sizeof(windowAttributes));

    windowAttributes.event_mask = KeyPressMask          //
                                | KeyReleaseMask        //
                                | ButtonPressMask       //
                                | ButtonReleaseMask     //
                                | PointerMotionMask     //
                                | EnterWindowMask       //
                                | LeaveWindowMask       //
                                | FocusChangeMask       //
                                | ExposureMask          //
                                | StructureNotifyMask   //
                                | PropertyChangeMask    //
                                | ColormapChangeMask    //
                                | VisibilityChangeMask  //
                                | SubstructureNotifyMask;

    window = XCreateWindow(display,
                           root,
                           0,
                           0,
                           window_width,
                           window_height,
                           0,
                           CopyFromParent,
                           InputOutput,
                           CopyFromParent,
                           CWEventMask,
                           &windowAttributes);

    XStoreName(display, window, window_title);
    XMapWindow(display, window);
    XFlush(display);

    platform_x11->display = display;
    platform_x11->window  = window;

    *platform = (void*)platform_x11;
    return egpoc_system_error_none;
}

egpoc_system_error_t egpoc_window_events(egpoc_window_t*       platform,
                                         egpoc_window_event_t* events,
                                         size_t                events_count_limit,
                                         size_t*               events_count)
{
    // TODO
    (void)events;

    egpoc_window_t* platform_x11 = (egpoc_window_t*)platform;

    Display* display = platform_x11->display;
    XEvent   event   = {.type = 0};

    size_t events_queued = (size_t)XEventsQueued(display, QueuedAfterReading);
    size_t events_count_ = (events_queued < events_count_limit) ? events_queued : events_count_limit;

    for (size_t i = 0; i < events_count_; i++) {
        XNextEvent(display, &event);

        // TODO: save events

        switch (event.type) {
        /* --- Generic any-event --- */
        default:
            EGPOC_DEBUG("Generic/Unknown event type: %d\n", event.type);
            EGPOC_DEBUG(
                "window=%lu, send_event=%d, serial=%lu\n", event.xany.window, event.xany.send_event, event.xany.serial);
            break;

        /* --- Key events --- */
        case KeyPress:
        case KeyRelease:
            EGPOC_DEBUG("%s:\n", event.type == KeyPress ? "KeyPress" : "KeyRelease");
            EGPOC_DEBUG(
                "  window=%lu, root=%lu, subwindow=%lu\n", event.xkey.window, event.xkey.root, event.xkey.subwindow);
            EGPOC_DEBUG("  time=%lu, x=%d, y=%d, x_root=%d, y_root=%d\n",
                        event.xkey.time,
                        event.xkey.x,
                        event.xkey.y,
                        event.xkey.x_root,
                        event.xkey.y_root);
            EGPOC_DEBUG("  state=0x%x, keycode=%u, same_screen=%d\n",
                        event.xkey.state,
                        event.xkey.keycode,
                        event.xkey.same_screen);
            break;

        /* --- Mouse buttons --- */
        case ButtonPress:
        case ButtonRelease:
            EGPOC_DEBUG("%s:\n", event.type == ButtonPress ? "ButtonPress" : "ButtonRelease");
            EGPOC_DEBUG("  button=%u, state=0x%x, x=%d, y=%d, root=%lu\n",
                        event.xbutton.button,
                        event.xbutton.state,
                        event.xbutton.x,
                        event.xbutton.y,
                        event.xbutton.root);
            EGPOC_DEBUG("  subwindow=%lu, same_screen=%d\n", event.xbutton.subwindow, event.xbutton.same_screen);
            break;

        /* --- Mouse motion --- */
        case MotionNotify:
            EGPOC_DEBUG("MotionNotify:\n");
            EGPOC_DEBUG("  x=%d, y=%d, x_root=%d, y_root=%d, state=0x%x, is_hint=%d\n",
                        event.xmotion.x,
                        event.xmotion.y,
                        event.xmotion.x_root,
                        event.xmotion.y_root,
                        event.xmotion.state,
                        event.xmotion.is_hint);
            break;

        /* --- Enter/Leave events --- */
        case EnterNotify:
        case LeaveNotify:
            EGPOC_DEBUG("%s:\n", event.type == EnterNotify ? "EnterNotify" : "LeaveNotify");
            EGPOC_DEBUG("  mode=%d, detail=%d, x=%d, y=%d, same_screen=%d, focus=%d\n",
                        event.xcrossing.mode,
                        event.xcrossing.detail,
                        event.xcrossing.x,
                        event.xcrossing.y,
                        event.xcrossing.same_screen,
                        event.xcrossing.focus);
            break;

        /* --- Focus events --- */
        case FocusIn:
        case FocusOut:
            EGPOC_DEBUG("%s: mode=%d, detail=%d\n",
                        event.type == FocusIn ? "FocusIn" : "FocusOut",
                        event.xfocus.mode,
                        event.xfocus.detail);
            break;

        /* --- Expose --- */
        case Expose:
            EGPOC_DEBUG("Expose:\n");
            EGPOC_DEBUG("  window=%lu, x=%d, y=%d, width=%d, height=%d, count=%d\n",
                        event.xexpose.window,
                        event.xexpose.x,
                        event.xexpose.y,
                        event.xexpose.width,
                        event.xexpose.height,
                        event.xexpose.count);
            break;

        /* --- Visibility --- */
        case VisibilityNotify:
            EGPOC_DEBUG("VisibilityNotify: state=%d\n", event.xvisibility.state);
            break;

        /* --- Window structure notifications --- */
        case CreateNotify:
            EGPOC_DEBUG("CreateNotify: window=%lu, parent=%lu, x=%d, y=%d, width=%d, height=%d\n",
                        event.xcreatewindow.window,
                        event.xcreatewindow.parent,
                        event.xcreatewindow.x,
                        event.xcreatewindow.y,
                        event.xcreatewindow.width,
                        event.xcreatewindow.height);
            break;

        case DestroyNotify:
            EGPOC_DEBUG(
                "DestroyNotify: event=%lu, window=%lu\n", event.xdestroywindow.event, event.xdestroywindow.window);
            break;

        case UnmapNotify:
            EGPOC_DEBUG("UnmapNotify: event=%lu, window=%lu\n", event.xunmap.event, event.xunmap.window);
            break;

        case MapNotify:
            EGPOC_DEBUG("MapNotify: event=%lu, window=%lu, override_redirect=%d\n",
                        event.xmap.event,
                        event.xmap.window,
                        event.xmap.override_redirect);
            break;

        case MapRequest:
            EGPOC_DEBUG("MapRequest: parent=%lu, window=%lu\n", event.xmaprequest.parent, event.xmaprequest.window);
            break;

        case ReparentNotify:
            EGPOC_DEBUG("ReparentNotify: event=%lu, window=%lu, parent=%lu, x=%d, y=%d\n",
                        event.xreparent.event,
                        event.xreparent.window,
                        event.xreparent.parent,
                        event.xreparent.x,
                        event.xreparent.y);
            break;

        case ConfigureNotify:
            EGPOC_DEBUG("ConfigureNotify: window=%lu, x=%d, y=%d, width=%d, height=%d, border=%d\n",
                        event.xconfigure.window,
                        event.xconfigure.x,
                        event.xconfigure.y,
                        event.xconfigure.width,
                        event.xconfigure.height,
                        event.xconfigure.border_width);
            break;

        case ConfigureRequest:
            EGPOC_DEBUG("ConfigureRequest: window=%lu, x=%d, y=%d, width=%d, height=%d, mask=0x%lx\n",
                        event.xconfigurerequest.window,
                        event.xconfigurerequest.x,
                        event.xconfigurerequest.y,
                        event.xconfigurerequest.width,
                        event.xconfigurerequest.height,
                        event.xconfigurerequest.value_mask);
            break;

        case GravityNotify:
            EGPOC_DEBUG(
                "GravityNotify: window=%lu, x=%d, y=%d\n", event.xgravity.window, event.xgravity.x, event.xgravity.y);
            break;

        case ResizeRequest:
            EGPOC_DEBUG(
                "ResizeRequest: width=%d, height=%d\n", event.xresizerequest.width, event.xresizerequest.height);
            break;

        case CirculateNotify:
            EGPOC_DEBUG("CirculateNotify: window=%lu, place=%d\n", event.xcirculate.window, event.xcirculate.place);
            break;

        case CirculateRequest:
            EGPOC_DEBUG("CirculateRequest: parent=%lu, window=%lu, place=%d\n",
                        event.xcirculaterequest.parent,
                        event.xcirculaterequest.window,
                        event.xcirculaterequest.place);
            break;

        /* --- Property & Selection events --- */
        case PropertyNotify:
            EGPOC_DEBUG("PropertyNotify: window=%lu, atom=%lu, state=%d, time=%lu\n",
                        event.xproperty.window,
                        event.xproperty.atom,
                        event.xproperty.state,
                        event.xproperty.time);
            break;

        case SelectionClear:
            EGPOC_DEBUG("SelectionClear: selection=%lu, time=%lu\n",
                        event.xselectionclear.selection,
                        event.xselectionclear.time);
            break;

        case SelectionRequest:
            EGPOC_DEBUG("SelectionRequest: requestor=%lu, selection=%lu, target=%lu, property=%lu\n",
                        event.xselectionrequest.requestor,
                        event.xselectionrequest.selection,
                        event.xselectionrequest.target,
                        event.xselectionrequest.property);
            break;

        case SelectionNotify:
            EGPOC_DEBUG("SelectionNotify: property=%lu, selection=%lu, target=%lu, time=%lu\n",
                        event.xselection.property,
                        event.xselection.selection,
                        event.xselection.target,
                        event.xselection.time);
            break;

        /* --- Colormap and Mapping events --- */
        case ColormapNotify:
            EGPOC_DEBUG("ColormapNotify: window=%lu, colormap=%lu, state=%d\n",
                        event.xcolormap.window,
                        event.xcolormap.colormap,
                        event.xcolormap.state);
            break;

        case MappingNotify:
            EGPOC_DEBUG("MappingNotify: request=%d, first_keycode=%d, count=%d\n",
                        event.xmapping.request,
                        event.xmapping.first_keycode,
                        event.xmapping.count);
            XRefreshKeyboardMapping(&event.xmapping);
            break;

        /* --- Client message --- */
        case ClientMessage:
            EGPOC_DEBUG("ClientMessage: window=%lu, format=%d, message_type=%lu\n",
                        event.xclient.window,
                        event.xclient.format,
                        event.xclient.message_type);
            EGPOC_DEBUG("  data: %ld %ld %ld %ld %ld\n",
                        event.xclient.data.l[0],
                        event.xclient.data.l[1],
                        event.xclient.data.l[2],
                        event.xclient.data.l[3],
                        event.xclient.data.l[4]);
            break;
        }
    }

    *events_count = events_count_;

    return egpoc_system_error_none;
}
