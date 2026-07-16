#if defined( _X11 )
#    include "Window/WindowPolicy/Linux/BorderlessGameLinuxPolicy.hpp"

#    include <X11/X.h>
#    include <X11/Xlib.h>
#    include <X11/Xatom.h>

namespace B33::App
{

using namespace B33::Core;
using namespace B33::Core::Debug;

// ---------------------------------------------------------------------------------------------------------------------
void BorderlessGameLinuxWindowPolicy::OnCreate( WindowDesc *pWd )
{
    B33_ASSERT( pWd );

    auto &windowData = pWd->Data;
    auto &windowOS   = pWd->OS;

    GameLinuxWindowPolicy::OnCreate( pWd );

    m_WMState      = XInternAtom( windowOS.pDisplayHandle, "_NET_WM_STATE", true );
    m_WMFullscreen = XInternAtom( windowOS.pDisplayHandle, "_NET_WM_STATE_FULLSCREEN", true );

    XChangeProperty( windowOS.pDisplayHandle,
                     windowOS.WindowHandle,
                     m_WMState,
                     XA_ATOM,
                     32,
                     PropModeReplace,
                     reinterpret_cast<unsigned char *>( &m_WMFullscreen ),
                     1 );

    XSync( windowOS.pDisplayHandle, false );
    XFlush( windowOS.pDisplayHandle );

    Window       dummyWindow;
    int          dummyInt;
    unsigned int dummyUInt;

    XGetGeometry( windowOS.pDisplayHandle,
                  windowOS.WindowHandle,
                  &dummyWindow,
                  &dummyInt,
                  &dummyInt,
                  reinterpret_cast<unsigned int *>( &( windowData.Width ) ),
                  reinterpret_cast<unsigned int *>( &( windowData.Height ) ),
                  &dummyUInt,
                  &dummyUInt );
}

} // namespace B33::App
#endif // !__linux__
