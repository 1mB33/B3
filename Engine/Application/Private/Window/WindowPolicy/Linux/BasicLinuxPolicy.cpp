#if defined( _X11 )

#    include "B33Core.h"

#    include "Input/InputEvents.h"
#    include "AppStatus.hpp"
#    include "Window/BaseWindowDetails.h"
#    include "Window/WindowPolicy/Linux/BasicLinuxPolicy.hpp"
#    include "X11ErrorHandling.hpp"

namespace B33::App
{

using namespace ::B33::Core;
using namespace ::B33::Core::Debug;

// ---------------------------------------------------------------------------------------------------------------------
u32 BasicLinuxWindowPolicy::CreateImpl( WindowDesc *pWd )
{
    B33_ASSERT( pWd );

    const auto &windowData = pWd->Data;
    auto       &windowOS   = pWd->OS;


    B33_ASSERT( windowData.Height > 0 );
    B33_ASSERT( windowData.Width > 0 );

    XSetErrorHandler( X11HandleError );
    windowOS.pDisplayHandle = AbAskForDisplayLinux( NULL );

    if ( windowOS.pDisplayHandle == NULL )
    {
        throw B33_EXCEPT( "pDisplayHandle is null!" );
    }

    int      screen   = DefaultScreen( windowOS.pDisplayHandle );
    Display *pDisplay = windowOS.pDisplayHandle;

    Window window = XCreateSimpleWindow( windowOS.pDisplayHandle,
                                         RootWindow( pDisplay, screen ),
                                         100,
                                         100,
                                         windowData.Width,
                                         windowData.Height,
                                         1,
                                         BlackPixel( pDisplay, screen ),
                                         WhitePixel( pDisplay, screen ) );

    XTextProperty windowName;
    char         *szWindowName = (char *)malloc( sizeof( char ) * B33_SMALL_STRING );
    size_t        uWriten      = wcstombs( szWindowName, windowData.Name.c_str(), windowData.Name.length() );
    szWindowName[ uWriten ]    = '\0';

    XStringListToTextProperty( &szWindowName, 1, &windowName );
    XSetWMName( pDisplay, window, &windowName );
    free( szWindowName );
    XFree( windowName.value );

    int bSupported;
    XkbSetDetectableAutoRepeat( pDisplay, True, &bSupported );
    if ( !bSupported )
    {
        B33_LOG( Error, L"Detectable auto repeat ISN'T SUPPORTED!" );
    }

    XSelectInput( pDisplay,
                  window,
                  FocusChangeMask | PointerMotionMask | PointerMotionHintMask | ButtonPressMask | ButtonReleaseMask |
                      KeyPressMask | KeyReleaseMask | ExposureMask | StructureNotifyMask | SubstructureNotifyMask |
                      SubstructureRedirectMask );
    windowOS.Screen       = screen;
    windowOS.WindowHandle = window;

    Atom wmDeleteMessage = XInternAtom( pDisplay, "WM_DELETE_WINDOW", 0 );
    XSetWMProtocols( pDisplay, window, &wmDeleteMessage, 1 );

    OnCreate( pWd );

    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicLinuxWindowPolicy::ShowImpl( WindowDesc *pWd )
{
    B33_ASSERT( pWd );

    __B33_ATTRIBUTE_MIGHT_BE_UNUSED const auto &windowData = pWd->Data;
    auto                                       &windowOS   = pWd->OS;


    B33_ASSERT( windowData.bIsAlive );
    B33_ASSERT( windowOS.pDisplayHandle );
    B33_ASSERT( windowOS.WindowHandle );

    XMapWindow( windowOS.pDisplayHandle, windowOS.WindowHandle );
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicLinuxWindowPolicy::HideImpl( WindowDesc *pWd )
{
    B33_ASSERT( pWd );

    __B33_ATTRIBUTE_MIGHT_BE_UNUSED const auto &windowData = pWd->Data;
    auto                                       &windowOS   = pWd->OS;


    B33_ASSERT( windowData.bIsAlive );
    B33_ASSERT( windowOS.pDisplayHandle );
    B33_ASSERT( windowOS.WindowHandle );

    XUnmapWindow( windowOS.pDisplayHandle, windowOS.WindowHandle );
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicLinuxWindowPolicy::DestroyImpl( WindowDesc *pWd )
{
    B33_ASSERT( pWd );

    __B33_ATTRIBUTE_MIGHT_BE_UNUSED const auto &windowData = pWd->Data;
    auto                                       &windowOS   = pWd->OS;


    B33_ASSERT( windowData.bIsAlive );
    B33_ASSERT( windowOS.pDisplayHandle );
    B33_ASSERT( windowOS.WindowHandle );

    XDestroyWindow( windowOS.pDisplayHandle, windowOS.WindowHandle );
    AbAskToCloseDisplayLinux( NULL );

    windowOS.WindowHandle   = 0;
    windowOS.pDisplayHandle = NULL;
    windowOS.Screen         = 0;
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicLinuxWindowPolicy::UpdateImpl( WindowDesc *pWd )
{
    B33_ASSERT( pWd );

    __B33_ATTRIBUTE_MIGHT_BE_UNUSED const auto &windowData = pWd->Data;
    auto                                       &windowOS   = pWd->OS;


    B33_ASSERT( windowData.bIsAlive );
    B33_ASSERT( windowOS.pDisplayHandle );
    B33_ASSERT( windowOS.WindowHandle );

    Display *display = windowOS.pDisplayHandle;
    Window   window  = windowOS.WindowHandle;
    XEvent   event;

    while ( XPending( display ) )
    {
        XPeekEvent( display, &event );

        if ( event.xany.window != window && event.type != UnmapNotify && event.type != DestroyNotify &&
             event.type != GenericEvent )
        {
            for ( const auto &handle : B33::App::AppStatus::Get().GetWindowHandles() )
            {
                if ( event.xany.window == handle->OS.WindowHandle )
                {
                    return;
                }
            }
        }

        XNextEvent( display, &event );

        if ( OnUpdate( pWd, event ) != 0 )
            break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------
u32 BasicLinuxWindowPolicy::OnUpdate( WindowDesc *pWd, XEvent &event )
{
    B33_ASSERT( pWd );

    auto &windowData = pWd->Data;
    auto &windowOS   = pWd->OS;


    B33_ASSERT( windowData.bIsAlive );
    B33_ASSERT( windowOS.pDisplayHandle );
    B33_ASSERT( windowOS.WindowHandle );

    Display *display = windowOS.pDisplayHandle;
    Window   window  = windowOS.WindowHandle;

    switch ( event.type )
    {
        case KeyPress:
            HandleKey( pWd, event, B33KeyPress );
            return 0;

        case KeyRelease:
            HandleKey( pWd, event, B33KeyRelease );
            return 0;

        case ButtonPress:
            HandleMouseButton( pWd, event, B33ButtonPress );
            return 0;

        case ButtonRelease:
            HandleMouseButton( pWd, event, B33ButtonRelease );
            return 0;

        case MotionNotify:
            int    rootX, rootY, dummy;
            Window dummyWindow;

            XQueryPointer( display,
                           window,
                           &dummyWindow,
                           &dummyWindow,
                           &rootX,
                           &rootY,
                           &dummy,
                           &dummy,
                           reinterpret_cast<unsigned int *>( &dummy ) );

            windowData.LastEvent |= Input;
            B33InputStruct is;

            is.Event        = B33Motion;
            is.Mouse.MouseX = static_cast<int32_t>( rootX );
            is.Mouse.MouseY = static_cast<int32_t>( rootY );

            windowData.InputStruct.push( is );
            return 0;

        case Expose:
            windowData.LastEvent |= Resize;
            windowData.Width  = event.xexpose.width;
            windowData.Height = event.xexpose.height;
            return 1;

        case ConfigureNotify:
            windowData.LastEvent |= Resize;
            windowData.Height = event.xconfigure.height;
            windowData.Width  = event.xconfigure.width;
            return 1;

        case ClientMessage:
            Atom wmDeleteMessage;

            if ( event.xclient.window != window )
                return 1;

            if ( ( wmDeleteMessage = XInternAtom( display, "WM_DELETE_WINDOW", 1 ) ) == None )
                break;

            if ( static_cast<Atom>( event.xclient.data.l[ 0 ] ) == wmDeleteMessage )
            {
                windowData.LastEvent |= Destroy;
                return 0;
            }

            return 0;
    }

    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicLinuxWindowPolicy::HandleKey( WindowDesc *pWd, XEvent &event, EB33InputEvents ie )
{
    pWd->Data.LastEvent |= Input;

    B33InputStruct is;
    is.Event          = ie;
    is.Keyboard.KeyId = event.xkey.keycode - 8;

    pWd->Data.InputStruct.push( is );
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicLinuxWindowPolicy::HandleMouseButton( WindowDesc *pWd, XEvent &event, EB33InputEvents ie )
{
    pWd->Data.LastEvent |= Input;

    B33InputStruct is;
    is.Event             = ie;
    is.MouseButton.KeyId = event.xbutton.button;

    pWd->Data.InputStruct.push( is );
}

} // namespace B33::App
#endif // !__linux__
