#include "egpoc_window.h"

#import <Cocoa/Cocoa.h>

#include "egpoc_debug.h"
#include "egpoc_system.h"

@interface EGPOCWindowDelegate : NSObject <NSWindowDelegate>
@property (nonatomic, assign) BOOL shouldQuit;
@end

@implementation EGPOCWindowDelegate
- (void)windowWillClose:(NSNotification *)notification {
    self.shouldQuit = YES;
}
@end

struct egpoc_window_t {
    NSWindow* window;
    EGPOCWindowDelegate* delegate;
    NSApplication* app;
};

egpoc_system_error_t egpoc_window_create(egpoc_memory_owner_t*  memory_owner,
                                         egpoc_memory_acquire_t memory_acquire,
                                         egpoc_memory_release_t memory_release,
                                         char const*            window_title,
                                         unsigned int           window_width,
                                         unsigned int           window_height,
                                         egpoc_window_t**       window)
{
    @autoreleasepool {
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

        NSRect rect = NSMakeRect(0, 0, window_width, window_height);
        NSWindowStyleMask style = NSWindowStyleMaskTitled
                                | NSWindowStyleMaskClosable
                                | NSWindowStyleMaskResizable;

        NSWindow* window_ns = [[NSWindow alloc] initWithContentRect:rect
                                                       styleMask:style
                                                         backing:NSBackingStoreBuffered
                                                           defer:NO];
        [window_ns setTitle:[NSString stringWithUTF8String:window_title]];
        [window_ns makeKeyAndOrderFront:nil];
        [NSApp activateIgnoringOtherApps:YES];

        EGPOCWindowDelegate* delegate = [[EGPOCWindowDelegate alloc] init];
        [window_ns setDelegate:delegate];

        egpoc_window_t* w = memory_acquire(memory_owner, sizeof(egpoc_window_t));
        if (!w) return egpoc_system_error_acquire;

        w->window = window_ns;
        w->delegate = delegate;
        w->app = NSApp;

        *window = w;
    }
    return 0;
}

egpoc_system_error_t egpoc_window_events(egpoc_window_t*       window,
                                         egpoc_window_event_t* events,
                                         size_t                events_count_limit,
                                         size_t*               events_count)
{
    if (!window || !events || !events_count) {
        return egpoc_system_error_argument;
    }

    @autoreleasepool {
        *events_count = 0;

        NSEvent* event;
        while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                            untilDate:[NSDate distantPast]
                                               inMode:NSDefaultRunLoopMode
                                              dequeue:YES])) {
            if (*events_count >= events_count_limit)
                break;

            egpoc_window_event_t* e = &events[*events_count];
            e->quit   = 0;
            e->mouseL = 0;
            e->mouseR = 0;
            e->mouseX = (int)[NSEvent mouseLocation].x;
            e->mouseY = (int)[NSEvent mouseLocation].y;

            // Print detailed event info
            switch (event.type) {
                case NSEventTypeLeftMouseDown:
                    EGPOC_DEBUG("[Event] LeftMouseDown at (%d,%d)\n", e->mouseX, e->mouseY);
                    e->mouseL = 1;
                    break;
                case NSEventTypeLeftMouseDragged:
                    EGPOC_DEBUG("[Event] LeftMouseDragged at (%d,%d)\n", e->mouseX, e->mouseY);
                    break;
                case NSEventTypeLeftMouseUp:
                    EGPOC_DEBUG("[Event] LeftMouseUp at (%d,%d)\n", e->mouseX, e->mouseY);
                    e->mouseL = 0;
                    break;
                case NSEventTypeRightMouseDown:
                    EGPOC_DEBUG("[Event] RightMouseDown at (%d,%d)\n", e->mouseX, e->mouseY);
                    e->mouseR = 1;
                    break;
                case NSEventTypeRightMouseUp:
                    EGPOC_DEBUG("[Event] RightMouseUp at (%d,%d)\n", e->mouseX, e->mouseY);
                    e->mouseR = 0;
                    break;
                case NSEventTypeRightMouseDragged:
                    EGPOC_DEBUG("[Event] RightMouseDragged at (%d,%d)\n", e->mouseX, e->mouseY);
                    break;
                case NSEventTypeMouseMoved:
                    EGPOC_DEBUG("[Event] MouseMoved at (%d,%d)\n", e->mouseX, e->mouseY);
                    break;
                case NSEventTypeMouseEntered:
                    EGPOC_DEBUG("[Event] MouseEntered\n");
                    break;
                case NSEventTypeMouseExited:
                    EGPOC_DEBUG("[Event] MouseExited\n");
                    break;
                case NSEventTypeKeyDown:
                    EGPOC_DEBUG("[Event] KeyDown: %s\n", [[event charactersIgnoringModifiers] UTF8String]);
                    break;
                case NSEventTypeKeyUp:
                    EGPOC_DEBUG("[Event] KeyUp: %s\n", [[event charactersIgnoringModifiers] UTF8String]);
                    break;
                case NSEventTypeScrollWheel:
                    EGPOC_DEBUG("[Event] ScrollWheel deltaY=%f deltaX=%f\n", event.scrollingDeltaY, event.scrollingDeltaX);
                    break;
                case NSEventTypeFlagsChanged:
                    EGPOC_DEBUG("[Event] Modifier flags changed: 0x%lx\n", (unsigned long)event.modifierFlags);
                    break;
                case NSEventTypeAppKitDefined:
                    EGPOC_DEBUG("[Event] AppKitDefined\n");
                    break;
                case NSEventTypeSystemDefined:
                    EGPOC_DEBUG("[Event] SystemDefined\n");
                    break;
                case NSEventTypeApplicationDefined:
                    EGPOC_DEBUG("[Event] ApplicationDefined\n");
                    break;
                case NSEventTypePressure:
                    EGPOC_DEBUG("[Event] Pressure = %f\n", event.pressure);
                    break;
                case NSEventTypeSwipe:
                    EGPOC_DEBUG("[Event] Swipe detected\n");
                    break;
                case NSEventTypeRotate:
                    EGPOC_DEBUG("[Event] Rotate gesture\n");
                    break;
                case NSEventTypeMagnify:
                    EGPOC_DEBUG("[Event] Magnify gesture scale = %f\n", event.magnification);
                    break;
                case NSEventTypeSmartMagnify:
                    EGPOC_DEBUG("[Event] SmartMagnify gesture\n");
                    break;
                case NSEventTypeBeginGesture:
                    EGPOC_DEBUG("[Event] BeginGesture\n");
                    break;
                case NSEventTypeEndGesture:
                    EGPOC_DEBUG("[Event] EndGesture\n");
                    break;
                case NSEventTypeTabletPoint:
                    EGPOC_DEBUG("[Event] TabletPoint\n");
                    break;
                case NSEventTypeTabletProximity:
                    EGPOC_DEBUG("[Event] TabletProximity\n");
                    break;
                case NSEventTypeCursorUpdate:
                    EGPOC_DEBUG("[Event] CursorUpdate\n");
                    break;
                case NSEventTypeChangeMode:
                    EGPOC_DEBUG("[Event] ChangeMode\n");
                    break;
                default:
                    EGPOC_DEBUG("[Event] Unhandled type %ld\n", (long)event.type);
                    break;
            }

            [NSApp sendEvent:event];
            [NSApp updateWindows];
            (*events_count)++;
        }
    }

    return egpoc_system_error_none;
}
