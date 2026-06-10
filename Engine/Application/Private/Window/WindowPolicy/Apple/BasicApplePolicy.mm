#include "Input/InputEvents.h"
#if defined( __APPLE__ )

#    include "B33Core.h"

#    include "AppStatus.hpp"
#    include "Window/BaseWindowDetails.h"
#    include "Window/WindowPolicy/Apple/BasicApplePolicy.hpp"
#    import <Cocoa/Cocoa.h>

namespace B33::App
{

using namespace ::B33::Core;
using namespace ::B33::Core::Debug;

// ---------------------------------------------------------------------------------------------------------------------
uint32_t BasicAppleWindowPolicy::CreateImpl( WindowDesc *pWd )
{
    NSRect frame = NSMakeRect(0, 0, 800, 600);
    NSUInteger style = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;
    NSWindow *pHandle = [[NSWindow alloc] initWithContentRect:frame styleMask:style backing:NSBackingStoreBuffered defer:NO];

    [pHandle setTitle:@"Unknown | fix me"];
    [pHandle center];
    [pHandle makeKeyWindow];
    [pHandle orderFrontRegardless];
    [pHandle setReleasedWhenClosed: NO];
    
    pWd->pWindow = (__bridge void*)pHandle;
    pWd->pMetalContext = nullptr;

    OnCreate(pWd);

    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicAppleWindowPolicy::ShowImpl( WindowDesc *pWd )
{
    B33_ASSERT( pWd );
    B33_ASSERT( pWd->bIsAlive );

    NSWindow *pHandle = (__bridge NSWindow*)pWd->pWindow;
    [pHandle makeKeyAndOrderFront: nil];
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicAppleWindowPolicy::HideImpl( WindowDesc *pWd )
{
    B33_ASSERT( pWd );
    B33_ASSERT( pWd->bIsAlive );

    NSWindow *pHandle = (__bridge NSWindow*)pWd->pWindow;
    [pHandle orderOut: nil];
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicAppleWindowPolicy::DestroyImpl( WindowDesc *pWd )
{
    B33_ASSERT( pWd );
    B33_ASSERT( pWd->bIsAlive );

    NSWindow *pHandle = (__bridge NSWindow*)pWd->pWindow;
    [pHandle close];

    pWd->pWindow = nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicAppleWindowPolicy::UpdateImpl( WindowDesc *pWd )
{
    B33_ASSERT( pWd );
    B33_ASSERT( pWd->bIsAlive );

    @autoreleasepool
    {
        NSEvent *event = nil;
        while ( ( event = [NSApp nextEventMatchingMask: NSEventMaskAny untilDate: nil inMode: NSDefaultRunLoopMode dequeue: YES] ) != nil)
        {
            switch ( event.type )
            {
            case NSEventTypeKeyDown:
            {
                pWd->LastEvent |= Input;

                AbInputStruct is;
                is.Event          = AbKeyPress;
                is.Keyboard.KeyId = event.keyCode + 1;

                pWd->InputStruct.push( is );
                break;
            }
            case NSEventTypeKeyUp:
            {
                pWd->LastEvent |= Input;

                AbInputStruct is;
                is.Event          = AbKeyRelease;
                is.Keyboard.KeyId = event.keyCode + 1;

                pWd->InputStruct.push( is );
                break;
            }
            case NSEventTypeMouseMoved:
            {
                pWd->LastEvent |= Input;

                CGFloat dx  = [event deltaX];
                CGFloat dy  = [event deltaY];

                AbInputStruct is;
                is.Event        = AbMotion;
                is.Mouse.MouseX = static_cast<int32_t>( dx );
                is.Mouse.MouseY = static_cast<int32_t>( dy );

                pWd->InputStruct.push( is );
                break;
            }
            default:
                [NSApp sendEvent: event];
                [NSApp updateWindows];
            };
        }
    }
}


} // namespace B33::App
#endif // !__linux__


