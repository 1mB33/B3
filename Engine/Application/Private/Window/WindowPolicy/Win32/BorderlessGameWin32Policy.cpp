#ifdef _WIN32

#    include "Window/BaseWindowDetails.h"
#    include "Window/WindowPolicy/Win32/BorderlessGameWin32Policy.hpp"
#    include <windowsx.h>

namespace B33::App
{

using namespace B33::Core;

void BorderlessGameWin32Policy::OnPreWcex()
{
    WindowDesc *pWd = this->GetWindowDesc();

    pWd->Data.pwszClassName = L"BorderlessGameAtlanticClass";

    memset( &pWd->OS.Wcex, 0, sizeof( WNDCLASSEX ) );

    pWd->OS.Wcex.cbSize        = sizeof( WNDCLASSEX );
    pWd->OS.Wcex.style         = CS_HREDRAW | CS_VREDRAW;
    pWd->OS.Wcex.hInstance     = GetModuleHandle( NULL );
    pWd->OS.Wcex.hCursor       = LoadCursor( NULL, IDC_ARROW );
    pWd->OS.Wcex.lpszClassName = pWd->Data.pwszClassName;
    pWd->OS.Wcex.lpfnWndProc   = WindowProc<WindowModeGameWin32WindowPolicy>;

    HMONITOR    hMonitor = MonitorFromWindow( pWd->OS.hWnd, MONITOR_DEFAULTTONEAREST );
    MONITORINFO mi;

    mi.cbSize = sizeof( mi );

    if ( GetMonitorInfo( hMonitor, &mi ) )
    {
        pWd->Data.Width  = mi.rcMonitor.right - mi.rcMonitor.left;
        pWd->Data.Height = mi.rcMonitor.bottom - mi.rcMonitor.top;
    }
}

void BorderlessGameWin32Policy::OnPreRegister()
{
    WindowDesc *pWd = this->GetWindowDesc();

    HWND hWnd = CreateWindowEx( WS_EX_APPWINDOW,
                                pWd->Data.pwszClassName,
                                pWd->Data.Name.c_str(),
                                WS_POPUP,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                pWd->Data.Width,
                                pWd->Data.Height,
                                NULL,
                                NULL,
                                GetModuleHandle( NULL ),
                                this );

    if ( hWnd == NULL )
    {
        B33_LOG( Core::Debug::Error, L"Couldn't CreateWindow(), last error %u", GetLastError() );
        return;
    }

    pWd->OS.hWnd = hWnd;
}
} // namespace B33::App
#endif // !_WIN32
