#ifndef B33_WINDOW_DESC_H
#define B33_WINDOW_DESC_H

#include "Input/InputEvents.h"
#include "WindowEvents.h"
#include <mutex>

/**
 * Struct that contains all the handles and information about the window.
 * Can be used to connect with different instances
 * of classes derived from WindowListener (like UserInput) [Engine/Application/Public/Window/WindowListener.hpp]
 * that require window handle.
 */
struct WindowDesc
{
    ::std::mutex       mUpdated;

    struct
    {
        ::std::wstring              Name;
        const wchar_t              *pwszClassName;
        int32_t                     Width;
        int32_t                     Height;
        bool                        bIsAlive;
        bool                        bIsVisible;
        EAbWindowEventsFlags        LastEvent;
        ::std::queue<AbInputStruct> InputStruct;
    } Data;

    struct
    {
#if defined( _WIN32 )
        HWND       hWnd;
        WNDCLASSEX Wcex;
#elif defined( _X11 )
        Display *pDisplayHandle;
        Window   WindowHandle;
        int32_t  Screen;
#elif defined( __APPLE__ )
        void *pWindow;
        void *pMetalContext;
#endif // !_WIN32
    } OS;

  public:
    WindowDesc() noexcept
      : mUpdated()
      , Data()
      , OS()
    {
    }

    WindowDesc( const WindowDesc &other )
      : mUpdated()
      , Data( other.Data )
      , OS( other.OS )
    {
    }
};

template <class U>
WindowDesc CreateWindowDesc( U &&wstrName, int32_t width = 1200, int32_t height = 700 )
{
    WindowDesc wd = {};

    wd.Data.Name          = ::std::forward<U>( wstrName );
    wd.Data.pwszClassName = NULL;
    wd.Data.Width         = width;
    wd.Data.Height        = height;
    wd.Data.bIsAlive      = false;
    wd.Data.bIsVisible    = false;
    wd.Data.LastEvent &= 0;

#if defined( _WIN32 )
    wd.OS.hWnd = NULL;
    wd.OS.Wcex = {};
#elif defined( _X11 )
    wd.OS.pDisplayHandle = NULL;
    wd.OS.WindowHandle   = 0;
    wd.OS.Screen         = 0;
#endif // !_WIN32
    return wd;
}

#endif // !B33_WINDOW_DESC_H
