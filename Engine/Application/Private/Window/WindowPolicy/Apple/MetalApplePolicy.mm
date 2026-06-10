#include "Input/InputEvents.h"
#if defined( __APPLE__ )

#    include "B33Core.h"

#    include "Window/WindowPolicy/Apple/MetalApplePolicy.hpp"
#    import <QuartzCore/CAMetalLayer.h>
#    import <Metal/Metal.h>
#    import <Cocoa/Cocoa.h>

namespace B33::App
{

using namespace ::B33::Core;
using namespace ::B33::Core::Debug;


void MetalAppleWindowPolicy::OnCreate( WindowDesc *pWd )
{   
    NSWindow *pHandle = (__bridge NSWindow*)pWd->pWindow;
    NSView *pView = [pHandle contentView];

    [pView setWantsLayer: YES];
    CAMetalLayer *pMetalLayer = [CAMetalLayer layer];
    pMetalLayer.frame = pView.bounds;
    pMetalLayer.opaque = YES;
    [pView setLayer: pMetalLayer];

    pWd->pMetalContext = (__bridge void*)pMetalLayer;
}

} // namespace B33::App
#endif // !__linux__



