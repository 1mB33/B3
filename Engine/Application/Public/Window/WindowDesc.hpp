#if !defined( B33_WINDOW_DESC_HPP )
#    define B33_WINDOW_DESC_HPP

#    include "Input/InputEvents.h"
#    include "WindowEvents.h"

/**
 * Struct that contains all the handles and information about the window.
 * Can be used to connect with different instances
 * of classes derived from WindowListener (like UserInput) [Engine/Application/Public/Window/WindowListener.hpp]
 * that require window handle.
 */
struct WindowDesc
{
    template <typename T>
    using Queue   = ::std::queue<T>;
    using Mutex   = ::std::mutex;
    using WString = ::std::wstring;

    Mutex mUpdated;

    struct
    {
        WString               Name;
        const wchar_t        *pwszClassName;
        i32                   Width;
        i32                   Height;
        bool                  bIsAlive;
        bool                  bIsVisible;
        EB33WindowEventsFlags LastEvent;
        Queue<B33InputStruct> InputStruct;
    } Data;

    struct
    {
#    if defined( _WIN32 )
        HWND       hWnd;
        WNDCLASSEX Wcex;
#    elif defined( _X11 )
        Display *pDisplayHandle;
        Window   WindowHandle;
        i32      Screen;
#    elif defined( __APPLE__ )
        void *pWindow;
        void *pMetalContext;
#    endif // !_WIN32
    } OS;

  public:
    WindowDesc() noexcept
      : mUpdated()
      , Data()
      , OS()
    {
    }

  public:
    WindowDesc( const WindowDesc &other )
      : mUpdated()
      , Data( other.Data )
      , OS( other.OS )
    {
    }

    WindowDesc &operator=( const WindowDesc &other )
    {
        Data = other.Data;
        OS   = other.OS;

        return *this;
    }

    WindowDesc( WindowDesc &&other ) noexcept
      : mUpdated()
      , Data( ::std::move( other.Data ) )
      , OS( ::std::move( other.OS ) )
    {
    }

    WindowDesc &operator=( WindowDesc &&other ) noexcept
    {
        Data = ::std::move( other.Data );
        OS   = ::std::move( other.OS );

        return *this;
    }
};

template <class U>
WindowDesc CreateWindowDesc( U &&wstrName, i32 width = 1200, i32 height = 700 )
{
    using ::std::forward;

    WindowDesc wd = {};

    wd.Data.Name          = forward<U>( wstrName );
    wd.Data.pwszClassName = NULL;
    wd.Data.Width         = width;
    wd.Data.Height        = height;
    wd.Data.bIsAlive      = false;
    wd.Data.bIsVisible    = false;
    wd.Data.LastEvent &= 0;

#    if defined( _WIN32 )
    wd.OS.hWnd = NULL;
    wd.OS.Wcex = {};
#    elif defined( _X11 )
    wd.OS.pDisplayHandle = NULL;
    wd.OS.WindowHandle   = 0;
    wd.OS.Screen         = 0;
#    endif // !_WIN32
    return wd;
}

#endif // !B33_WINDOW_DESC_HPP
