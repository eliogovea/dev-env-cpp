#include "egpoc_system.h"
#include "egpoc_window.h"

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "egpoc_system.h"

typedef struct egpoc_window_t {
    egpoc_system_error_t error;
} egpoc_window_t;

static inline void egpoc_console_printf(char const* format, ...) __attribute__((format(printf, 1, 2)));

static inline void egpoc_console_printf(char const* format, ...)
{
    char    buffer[512];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"
#pragma clang diagnostic ignored "-Wvariadic-macro-arguments-omitted"
    EM_ASM({ console.log(UTF8ToString($0)); }, buffer);
#pragma clang diagnostic pop
}

/* ------------------------------------------------------------------------- */
/* Keyboard events                                                           */
/* ------------------------------------------------------------------------- */
static EM_BOOL egpoc_on_event_key(int type, EmscriptenKeyboardEvent const* e, void* userData)
{
    (void)userData;

    switch (type) {
    case EMSCRIPTEN_EVENT_KEYDOWN: {
        egpoc_console_printf("[Key] keydown:"
                             " key=%s code=%s charValue=%s locale=%s"
                             " keyCode=%u charCode=%u which=%u location=%u"
                             " ctrl=%d shift=%d alt=%d meta=%d repeat=%d timestamp=%.3f",
                             e->key,
                             e->code,
                             e->charValue,
                             e->locale,
                             e->keyCode,
                             e->charCode,
                             e->which,
                             e->location,
                             e->ctrlKey,
                             e->shiftKey,
                             e->altKey,
                             e->metaKey,
                             e->repeat,
                             e->timestamp);
        // TODO: update egpoc_window_event_t
    } break;

    case EMSCRIPTEN_EVENT_KEYUP: {
        egpoc_console_printf("[Key] keyup:"
                             " key=%s code=%s charValue=%s locale=%s"
                             " keyCode=%u charCode=%u which=%u location=%u"
                             " ctrl=%d shift=%d alt=%d meta=%d repeat=%d timestamp=%.3f",
                             e->key,
                             e->code,
                             e->charValue,
                             e->locale,
                             e->keyCode,
                             e->charCode,
                             e->which,
                             e->location,
                             e->ctrlKey,
                             e->shiftKey,
                             e->altKey,
                             e->metaKey,
                             e->repeat,
                             e->timestamp);
        // TODO: update egpoc_window_event_t
    } break;

    case EMSCRIPTEN_EVENT_KEYPRESS: {
        egpoc_console_printf("[Key] keypress:"
                             " key=%s code=%s charValue=%s locale=%s"
                             " keyCode=%u charCode=%u which=%u location=%u"
                             " ctrl=%d shift=%d alt=%d meta=%d repeat=%d timestamp=%.3f",
                             e->key,
                             e->code,
                             e->charValue,
                             e->locale,
                             e->keyCode,
                             e->charCode,
                             e->which,
                             e->location,
                             e->ctrlKey,
                             e->shiftKey,
                             e->altKey,
                             e->metaKey,
                             e->repeat,
                             e->timestamp);
        // TODO: update egpoc_window_event_t
    } break;

    default:
        break;
    }

    return EM_TRUE;
}

/* ------------------------------------------------------------------------- */
/* Mouse events                                                              */
/* ------------------------------------------------------------------------- */
static EM_BOOL egpoc_on_event_mouse(int type, EmscriptenMouseEvent const* e, void* userData)
{
    (void)userData;

    switch (type) {
    case EMSCRIPTEN_EVENT_CLICK: {
        egpoc_console_printf("[Mouse] click:"
                             " time=%.3f"
                             " button=%u buttons=%u"
                             " screen=(%d,%d)"
                             " client=(%d,%d)"
                             " target=(%d,%d)"
                             " movement=(%d,%d)"
                             " ctrl=%d shift=%d alt=%d meta=%d",
                             e->timestamp,
                             e->button,
                             e->buttons,
                             e->screenX,
                             e->screenY,
                             e->clientX,
                             e->clientY,
                             e->targetX,
                             e->targetY,
                             e->movementX,
                             e->movementY,
                             e->ctrlKey,
                             e->shiftKey,
                             e->altKey,
                             e->metaKey);
        // TODO: update egpoc_window_event_t
    } break;

    case EMSCRIPTEN_EVENT_DBLCLICK: {
        egpoc_console_printf("[Mouse] dblclick:"
                             " time=%.3f"
                             " button=%u buttons=%u"
                             " screen=(%d,%d)"
                             " client=(%d,%d)"
                             " target=(%d,%d)"
                             " movement=(%d,%d)"
                             " ctrl=%d shift=%d alt=%d meta=%d",
                             e->timestamp,
                             e->button,
                             e->buttons,
                             e->screenX,
                             e->screenY,
                             e->clientX,
                             e->clientY,
                             e->targetX,
                             e->targetY,
                             e->movementX,
                             e->movementY,
                             e->ctrlKey,
                             e->shiftKey,
                             e->altKey,
                             e->metaKey);
        // TODO: update egpoc_window_event_t
    } break;

    case EMSCRIPTEN_EVENT_MOUSEDOWN: {
        egpoc_console_printf("[Mouse] mousedown:"
                             " time=%.3f"
                             " button=%u buttons=%u"
                             " screen=(%d,%d)"
                             " client=(%d,%d)"
                             " target=(%d,%d)"
                             " movement=(%d,%d)"
                             " ctrl=%d shift=%d alt=%d meta=%d",
                             e->timestamp,
                             e->button,
                             e->buttons,
                             e->screenX,
                             e->screenY,
                             e->clientX,
                             e->clientY,
                             e->targetX,
                             e->targetY,
                             e->movementX,
                             e->movementY,
                             e->ctrlKey,
                             e->shiftKey,
                             e->altKey,
                             e->metaKey);
        // TODO: update egpoc_window_event_t
    } break;

    case EMSCRIPTEN_EVENT_MOUSEUP: {
        egpoc_console_printf("[Mouse] mouseup:"
                             " time=%.3f"
                             " button=%u buttons=%u"
                             " screen=(%d,%d)"
                             " client=(%d,%d)"
                             " target=(%d,%d)"
                             " movement=(%d,%d)"
                             " ctrl=%d shift=%d alt=%d meta=%d",
                             e->timestamp,
                             e->button,
                             e->buttons,
                             e->screenX,
                             e->screenY,
                             e->clientX,
                             e->clientY,
                             e->targetX,
                             e->targetY,
                             e->movementX,
                             e->movementY,
                             e->ctrlKey,
                             e->shiftKey,
                             e->altKey,
                             e->metaKey);
        // TODO: update egpoc_window_event_t
    } break;

    case EMSCRIPTEN_EVENT_MOUSEMOVE: {
        egpoc_console_printf("[Mouse] mousemove:"
                             " time=%.3f"
                             " buttons=%u"
                             " screen=(%d,%d)"
                             " client=(%d,%d)"
                             " target=(%d,%d)"
                             " movement=(%d,%d)"
                             " ctrl=%d shift=%d alt=%d meta=%d",
                             e->timestamp,
                             e->buttons,
                             e->screenX,
                             e->screenY,
                             e->clientX,
                             e->clientY,
                             e->targetX,
                             e->targetY,
                             e->movementX,
                             e->movementY,
                             e->ctrlKey,
                             e->shiftKey,
                             e->altKey,
                             e->metaKey);
        // TODO: update egpoc_window_event_t
    } break;

    case EMSCRIPTEN_EVENT_MOUSEENTER: {
        egpoc_console_printf("[Mouse] mouseenter:"
                             " time=%.3f"
                             " client=(%d,%d) target=(%d,%d)"
                             " ctrl=%d shift=%d alt=%d meta=%d",
                             e->timestamp,
                             e->clientX,
                             e->clientY,
                             e->targetX,
                             e->targetY,
                             e->ctrlKey,
                             e->shiftKey,
                             e->altKey,
                             e->metaKey);
        // TODO: update egpoc_window_event_t
    } break;

    case EMSCRIPTEN_EVENT_MOUSELEAVE: {
        egpoc_console_printf("[Mouse] mouseleave:"
                             " time=%.3f"
                             " client=(%d,%d) target=(%d,%d)"
                             " ctrl=%d shift=%d alt=%d meta=%d",
                             e->timestamp,
                             e->clientX,
                             e->clientY,
                             e->targetX,
                             e->targetY,
                             e->ctrlKey,
                             e->shiftKey,
                             e->altKey,
                             e->metaKey);
        // TODO: update egpoc_window_event_t
    } break;

    case EMSCRIPTEN_EVENT_MOUSEOVER: {
        egpoc_console_printf("[Mouse] mouseover:"
                             " time=%.3f"
                             " client=(%d,%d) target=(%d,%d)"
                             " ctrl=%d shift=%d alt=%d meta=%d",
                             e->timestamp,
                             e->clientX,
                             e->clientY,
                             e->targetX,
                             e->targetY,
                             e->ctrlKey,
                             e->shiftKey,
                             e->altKey,
                             e->metaKey);
        // TODO: update egpoc_window_event_t
    } break;

    case EMSCRIPTEN_EVENT_MOUSEOUT: {
        egpoc_console_printf("[Mouse] mouseout:"
                             " time=%.3f"
                             " client=(%d,%d) target=(%d,%d)"
                             " ctrl=%d shift=%d alt=%d meta=%d",
                             e->timestamp,
                             e->clientX,
                             e->clientY,
                             e->targetX,
                             e->targetY,
                             e->ctrlKey,
                             e->shiftKey,
                             e->altKey,
                             e->metaKey);
        // TODO: update egpoc_window_event_t
    } break;

    default:
        break;
    }

    return EM_TRUE;
}

/* ------------------------------------------------------------------------- */
/* Wheel events                                                              */
/* ------------------------------------------------------------------------- */
static EM_BOOL egpoc_on_event_wheel(int type, EmscriptenWheelEvent const* e, void* userData)
{
    (void)userData;

    switch (type) {
    case EMSCRIPTEN_EVENT_WHEEL: {
        egpoc_console_printf(
            "[Wheel] deltaX=%lf deltaY=%lf deltaZ=%lf mode=%u", e->deltaX, e->deltaY, e->deltaZ, e->deltaMode);
        // TODO: update egpoc_window_event_t with wheel delta
    } break;
    default:
        break;
    }
    return EM_TRUE;
}

/* ------------------------------------------------------------------------- */
/* Touch events                                                              */
/* ------------------------------------------------------------------------- */
static EM_BOOL egpoc_on_event_touch(int type, EmscriptenTouchEvent const* e, void* userData)
{
    (void)userData;

    switch (type) {
    case EMSCRIPTEN_EVENT_TOUCHSTART: {
        egpoc_console_printf("[Touch] touchstart:"
                             " time=%.3f numTouches=%d ctrl=%d shift=%d alt=%d meta=%d",
                             e->timestamp,
                             e->numTouches,
                             e->ctrlKey,
                             e->shiftKey,
                             e->altKey,
                             e->metaKey);
        for (int i = 0; i < e->numTouches && i < 32; ++i) {
            EmscriptenTouchPoint const* t = &e->touches[i];
            egpoc_console_printf("   id=%d isChanged=%d onTarget=%d"
                                 " screen=(%d,%d) client=(%d,%d) target=(%d,%d)",
                                 t->identifier,
                                 t->isChanged,
                                 t->onTarget,
                                 t->screenX,
                                 t->screenY,
                                 t->clientX,
                                 t->clientY,
                                 t->targetX,
                                 t->targetY);
        }
        // TODO: update egpoc_window_event_t
    } break;

    case EMSCRIPTEN_EVENT_TOUCHEND: {
        egpoc_console_printf("[Touch] touchend:"
                             " time=%.3f numTouches=%d ctrl=%d shift=%d alt=%d meta=%d",
                             e->timestamp,
                             e->numTouches,
                             e->ctrlKey,
                             e->shiftKey,
                             e->altKey,
                             e->metaKey);
        for (int i = 0; i < e->numTouches && i < 32; ++i) {
            EmscriptenTouchPoint const* t = &e->touches[i];
            egpoc_console_printf("   id=%d isChanged=%d onTarget=%d"
                                 " screen=(%d,%d) client=(%d,%d) target=(%d,%d)",
                                 t->identifier,
                                 t->isChanged,
                                 t->onTarget,
                                 t->screenX,
                                 t->screenY,
                                 t->clientX,
                                 t->clientY,
                                 t->targetX,
                                 t->targetY);
        }
        // TODO: update egpoc_window_event_t
    } break;

    case EMSCRIPTEN_EVENT_TOUCHMOVE: {
        egpoc_console_printf("[Touch] touchmove:"
                             " time=%.3f numTouches=%d ctrl=%d shift=%d alt=%d meta=%d",
                             e->timestamp,
                             e->numTouches,
                             e->ctrlKey,
                             e->shiftKey,
                             e->altKey,
                             e->metaKey);
        for (int i = 0; i < e->numTouches && i < 32; ++i) {
            EmscriptenTouchPoint const* t = &e->touches[i];
            egpoc_console_printf("   id=%d isChanged=%d onTarget=%d"
                                 " screen=(%d,%d) client=(%d,%d) target=(%d,%d)",
                                 t->identifier,
                                 t->isChanged,
                                 t->onTarget,
                                 t->screenX,
                                 t->screenY,
                                 t->clientX,
                                 t->clientY,
                                 t->targetX,
                                 t->targetY);
        }
        // TODO: update egpoc_window_event_t
    } break;

    case EMSCRIPTEN_EVENT_TOUCHCANCEL: {
        egpoc_console_printf("[Touch] touchcancel:"
                             " time=%.3f numTouches=%d ctrl=%d shift=%d alt=%d meta=%d",
                             e->timestamp,
                             e->numTouches,
                             e->ctrlKey,
                             e->shiftKey,
                             e->altKey,
                             e->metaKey);
        for (int i = 0; i < e->numTouches && i < 32; ++i) {
            EmscriptenTouchPoint const* t = &e->touches[i];
            egpoc_console_printf("   id=%d isChanged=%d onTarget=%d"
                                 " screen=(%d,%d) client=(%d,%d) target=(%d,%d)",
                                 t->identifier,
                                 t->isChanged,
                                 t->onTarget,
                                 t->screenX,
                                 t->screenY,
                                 t->clientX,
                                 t->clientY,
                                 t->targetX,
                                 t->targetY);
        }
        // TODO: update egpoc_window_event_t
    } break;

    default:
        break;
    }

    return EM_TRUE;
}

/* ------------------------------------------------------------------------- */
/* Focus events                                                              */
/* ------------------------------------------------------------------------- */
static EM_BOOL egpoc_on_event_focus(int type, EmscriptenFocusEvent const* e, void* userData)
{
    (void)userData;

    switch (type) {
    case EMSCRIPTEN_EVENT_BLUR: {
        egpoc_console_printf("[Focus] blur:"
                             " nodeName=%s id=%s",
                             e->nodeName,
                             e->id);
        // TODO: update egpoc_window_event_t
    } break;

    case EMSCRIPTEN_EVENT_FOCUS: {
        egpoc_console_printf("[Focus] focus:"
                             " nodeName=%s id=%s",
                             e->nodeName,
                             e->id);
        // TODO: update egpoc_window_event_t
    } break;

    case EMSCRIPTEN_EVENT_FOCUSIN: {
        egpoc_console_printf("[Focus] focusin:"
                             " nodeName=%s id=%s",
                             e->nodeName,
                             e->id);
        // TODO: update egpoc_window_event_t
    } break;

    case EMSCRIPTEN_EVENT_FOCUSOUT: {
        egpoc_console_printf("[Focus] focusout:"
                             " nodeName=%s id=%s",
                             e->nodeName,
                             e->id);
        // TODO: update egpoc_window_event_t
    } break;

    default:
        break;
    }

    return EM_TRUE;
}

/* ------------------------------------------------------------------------- */
/* UI events                                                                 */
/* ------------------------------------------------------------------------- */
static EM_BOOL egpoc_on_event_ui(int type, EmscriptenUiEvent const* e, void* userData)
{
    (void)userData;

    switch (type) {
    case EMSCRIPTEN_EVENT_RESIZE: {
        egpoc_console_printf("[UI] resize:"
                             " detail=%d"
                             " bodyClient=%dx%d"
                             " windowInner=%dx%d"
                             " windowOuter=%dx%d"
                             " scrollTop=%d scrollLeft=%d",
                             e->detail,
                             e->documentBodyClientWidth,
                             e->documentBodyClientHeight,
                             e->windowInnerWidth,
                             e->windowInnerHeight,
                             e->windowOuterWidth,
                             e->windowOuterHeight,
                             e->scrollTop,
                             e->scrollLeft);
        // TODO: update egpoc_window_event_t
    } break;

    case EMSCRIPTEN_EVENT_SCROLL: {
        egpoc_console_printf("[UI] scroll:"
                             " detail=%d"
                             " scrollTop=%d scrollLeft=%d"
                             " windowInner=%dx%d"
                             " windowOuter=%dx%d",
                             e->detail,
                             e->scrollTop,
                             e->scrollLeft,
                             e->windowInnerWidth,
                             e->windowInnerHeight,
                             e->windowOuterWidth,
                             e->windowOuterHeight);
        // TODO: update egpoc_window_event_t
    } break;

    default:
        break;
    }

    return EM_TRUE;
}

/* ------------------------------------------------------------------------- */
/* Device orientation                                                        */
/* ------------------------------------------------------------------------- */
// static EM_BOOL egpoc_on_event_deviceorientation(int type, EmscriptenDeviceOrientationEvent const* e, void* userData)
// {
//     (void)userData;

//     switch (type) {
//     case EMSCRIPTEN_EVENT_DEVICEORIENTATION: {
//         egpoc_console_printf(
//             "[DeviceOrientation] alpha=%lf beta=%lf gamma=%lf abs=%d", e->alpha, e->beta, e->gamma, e->absolute);
//         // TODO: update egpoc_window_event_t
//     } break;
//     default:
//         break;
//     }
//     return EM_TRUE;
// }

/* ------------------------------------------------------------------------- */
/* Device motion                                                             */
/* ------------------------------------------------------------------------- */
// static EM_BOOL egpoc_on_event_devicemotion(int type, EmscriptenDeviceMotionEvent const* e, void* userData)
// {
//     (void)userData;

//     switch (type) {
//     case EMSCRIPTEN_EVENT_DEVICEMOTION: {
//         egpoc_console_printf("[DeviceMotion] acc=(%.3f, %.3f, %.3f) accG=(%.3f, %.3f, %.3f)"
//                              " rotation=(%.3f, %.3f, %.3f) supportedFields=0x%x",
//                              e->accelerationX,
//                              e->accelerationY,
//                              e->accelerationZ,
//                              e->accelerationIncludingGravityX,
//                              e->accelerationIncludingGravityY,
//                              e->accelerationIncludingGravityZ,
//                              e->rotationRateAlpha,
//                              e->rotationRateBeta,
//                              e->rotationRateGamma,
//                              e->supportedFields);
//         // TODO: update egpoc_window_event_t
//     } break;

//     default:
//         break;
//     }

//     return EM_TRUE;
// }

/* ------------------------------------------------------------------------- */
/* Visibility events                                                         */
/* ------------------------------------------------------------------------- */
// static EM_BOOL egpoc_on_event_visibility(int type, EmscriptenVisibilityChangeEvent const* e, void* userData)
// {
//     (void)userData;

//     switch (type) {
//     case EMSCRIPTEN_EVENT_VISIBILITYCHANGE: {
//         egpoc_console_printf("[Visibility] hidden=%d visibilityState=%d", e->hidden, e->visibilityState);
//         // TODO: update egpoc_window_event_t
//     } break;

//     default:
//         break;
//     }

//     return EM_TRUE;
// }

/* ------------------------------------------------------------------------- */
/* Gamepad events                                                            */
/* ------------------------------------------------------------------------- */
// static EM_BOOL egpoc_on_event_gamepad(int type, EmscriptenGamepadEvent const* e, void* userData)
// {
//     (void)userData;

//     switch (type) {
//     case EMSCRIPTEN_EVENT_GAMEPADCONNECTED: {
//         egpoc_console_printf("[Gamepad] connected:"
//                              " time=%.3f index=%d id=%s mapping=%s"
//                              " axes=%d buttons=%d connected=%d",
//                              e->timestamp,
//                              e->index,
//                              e->id,
//                              e->mapping,
//                              e->numAxes,
//                              e->numButtons,
//                              e->connected);

//         // Print axis values (up to 64)
//         for (int i = 0; i < e->numAxes && i < 64; ++i) {
//             egpoc_console_printf("   axis[%d] = %.3f", i, e->axis[i]);
//         }

//         // Print button values (up to 64)
//         for (int i = 0; i < e->numButtons && i < 64; ++i) {
//             egpoc_console_printf("   button[%d] = %.3f (digital=%d)", i, e->analogButton[i], e->digitalButton[i]);
//         }

//         // TODO: update egpoc_window_event_t
//     } break;

//     case EMSCRIPTEN_EVENT_GAMEPADDISCONNECTED: {
//         egpoc_console_printf("[Gamepad] disconnected:"
//                              " time=%.3f index=%d id=%s mapping=%s connected=%d",
//                              e->timestamp,
//                              e->index,
//                              e->id,
//                              e->mapping,
//                              e->connected);
//         // TODO: update egpoc_window_event_t
//     } break;

//     default:
//         break;
//     }

//     return EM_TRUE;
// }

EMSCRIPTEN_KEEPALIVE
egpoc_system_error_t egpoc_window_create(egpoc_memory_owner_t*  memory_owner,
                                         egpoc_memory_acquire_t memory_acquire,
                                         egpoc_memory_release_t memory_release,
                                         char const*            window_title,
                                         unsigned int           window_width,
                                         unsigned int           window_height,
                                         egpoc_window_t**       platform)
{
    egpoc_window_t* platform_wasm = (egpoc_window_t*)memory_acquire(memory_owner, sizeof(egpoc_window_t));

    if (!platform_wasm) {
        platform_wasm = memory_release(memory_owner, sizeof(egpoc_window_t), (void*)platform_wasm);
        return egpoc_system_error_unknown;
    }

    memset(platform_wasm, 0, sizeof(*platform_wasm));

    char js[512];

    snprintf(js,
             sizeof(js),
             "let c=document.getElementById('egpoc_canvas');"
             "if(!c){c=document.createElement('canvas');c.id='egpoc_canvas';"
             "c.style.border='1px solid #333';document.body.appendChild(c);}"
             "c.width=%d; c.height=%d; document.title='%s';"
             "c.oncontextmenu=(e)=>e.preventDefault();"
             "var canvas = document.getElementById('egpoc_canvas');"
             "canvas.setAttribute('tabindex', '0');"
             "canvas.focus();",
             window_width,
             window_height,
             window_title ? window_title : "Egpoc WASM");

    emscripten_run_script(js);

    emscripten_set_keypress_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_key);
    emscripten_set_keydown_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_key);
    emscripten_set_keyup_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_key);

    emscripten_set_click_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_mouse);
    emscripten_set_dblclick_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_mouse);
    emscripten_set_mousedown_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_mouse);
    emscripten_set_mouseup_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_mouse);
    emscripten_set_mousemove_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_mouse);
    emscripten_set_mouseenter_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_mouse);
    emscripten_set_mouseleave_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_mouse);
    emscripten_set_mouseover_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_mouse);
    emscripten_set_mouseout_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_mouse);
    emscripten_set_wheel_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_wheel);

    emscripten_set_touchstart_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_touch);
    emscripten_set_touchend_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_touch);
    emscripten_set_touchmove_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_touch);
    emscripten_set_touchcancel_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_touch);

    emscripten_set_blur_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_focus);
    emscripten_set_focus_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_focus);
    emscripten_set_focusin_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_focus);
    emscripten_set_focusout_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_focus);

    emscripten_set_resize_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_ui);
    emscripten_set_scroll_callback("#egpoc_canvas", NULL, EM_TRUE, egpoc_on_event_ui);

    *platform = (egpoc_window_t*)platform_wasm;

    return egpoc_system_error_none;
}

EMSCRIPTEN_KEEPALIVE
egpoc_system_error_t egpoc_window_events(egpoc_window_t*       platform,
                                         egpoc_window_event_t* events,
                                         size_t                events_count_limit,
                                         size_t*               events_count)
{
    (void)events_count_limit;

    egpoc_window_t* platform_wasm = (egpoc_window_t*)platform;

    if (!platform_wasm) {
        return egpoc_system_error_unknown;
    }

    if (!events) {
        return egpoc_system_error_unknown;
    }

    if (!events_count) {
        return egpoc_system_error_unknown;
    }

    *events_count = 0;

    return egpoc_system_error_none;
}
