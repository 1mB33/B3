#if defined( __APPLE__ )

#    include "B33System.hpp"

#    include "EntryPoints/Apple/AppleRunningUtils.hpp"
#    import <Cocoa/Cocoa.h>

namespace B33::System
{

using namespace ::B33::Core;
using namespace ::B33::Core::Debug;

// ---------------------------------------------------------------------------------------------------------------------
void RunningUtils::StartUpNSApp()
{
    [NSApplication sharedApplication];
    [NSApp setActivationPolicy: NSApplicationActivationPolicyRegular];
    [NSApp finishLaunching];
    [NSApp activateIgnoringOtherApps: YES];
}

} // namespace B33::System
#endif // !__APPLE__



