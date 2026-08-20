#ifdef _WIN32

#    include "Window/WindowPolicy/Win32/WindowModeGameWin32Policy.hpp"
#    include "MinimalWindowsIncludes.h"
#    include <windowsx.h>
#    include <hidusage.h>

namespace B33::App
{

using namespace std;
using namespace B33::Core;

// ---------------------------------------------------------------------------------------------------------------------
void WindowModeGameWin32WindowPolicy::OnPreWcex()
{
    WindowDesc *pWd = this->GetWindowDesc();

    pWd->Data.pwszClassName = L"GameB3Class";

    memset( &pWd->OS.Wcex, 0, sizeof( WNDCLASSEX ) );

    pWd->OS.Wcex.cbSize        = sizeof( WNDCLASSEX );
    pWd->OS.Wcex.style         = CS_HREDRAW | CS_VREDRAW;
    pWd->OS.Wcex.hInstance     = GetModuleHandle( NULL );
    pWd->OS.Wcex.hCursor       = LoadCursor( NULL, IDC_ARROW );
    pWd->OS.Wcex.lpszClassName = pWd->Data.pwszClassName;
    pWd->OS.Wcex.lpfnWndProc   = WindowProc<WindowModeGameWin32WindowPolicy>;
}

typedef __int64 QWORD;

// ---------------------------------------------------------------------------------------------------------------------
void WindowModeGameWin32WindowPolicy::OnUpdate( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    WindowDesc *pWd = this->GetWindowDesc();

    switch ( uMsg )
    {
        case WM_CREATE:
        {
            SetFocus( pWd->OS.hWnd );
            SetForegroundWindow( pWd->OS.hWnd );
            break;
        }
        case WM_SHOWWINDOW:
            // Window is being hidden
            if ( wParam != TRUE )
            {
                break;
            }
            [[fallthrough]];
        case WM_SETFOCUS:
        {
            B33_LOG( B33::Core::Debug::Info, L"Capturing focus" );

            RECT rect;
            GetWindowRect( pWd->OS.hWnd, &rect );
            rect.top += 50;
            rect.bottom -= 15;
            rect.left += 15;
            rect.right -= 15;
            SetCursorPos( static_cast<int>( rect.left + 0.5f * pWd->Data.Width ),
                          static_cast<int>( rect.top + 0.5f * pWd->Data.Height ) );
            ClipCursor( &rect );

            ShowCursor( FALSE );

            RAWINPUTDEVICE rid;

            rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
            rid.usUsage     = HID_USAGE_GENERIC_MOUSE;
            rid.dwFlags     = RIDEV_INPUTSINK;
            rid.hwndTarget  = pWd->OS.hWnd;

            if ( !RegisterRawInputDevices( &rid, 1, sizeof( rid ) ) )
            {
                B33_LOG( B33::Core::Debug::Error, L"Couldn't register raw input" );
            }

            break;
        }

        case WM_KILLFOCUS:
        case WM_DESTROY:
        {
            B33_LOG( B33::Core::Debug::Info, L"Leaving focus" );
            RAWINPUTDEVICE rid;

            rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
            rid.usUsage     = HID_USAGE_GENERIC_MOUSE;
            rid.dwFlags     = RIDEV_REMOVE;
            rid.hwndTarget  = NULL;

            if ( !RegisterRawInputDevices( &rid, 1, sizeof( rid ) ) )
                throw B33_EXCEPT( "Couldn't register raw input" );

            ShowCursor( TRUE );
            ClipCursor( NULL );
            break;
        }
        case WM_MOUSEMOVE:
        {
            return;
        }

        case WM_INPUT:
        {
            static ::std::vector<BYTE> vRi = {};
            AbInputStruct              is  = {};
            UINT                       cbSize;
            UINT                       cbSize2;
            size_t                     uRiRead;
            RAWINPUT                  *pRi;

            if ( GetRawInputBuffer( NULL, &cbSize, sizeof( RAWINPUTHEADER ) ) != 0 )
            {
                B33_LOG( B33::Core::Debug::Error, L"GetRawInputBuffer error %d", GetLastError() );
                return;
            }

            if (cbSize == 0)
            {
                return;
            }

            cbSize2 = (cbSize + 1) * 16;
            if ( vRi.size() < cbSize2 )
                vRi.resize( cbSize2 );

            uRiRead = GetRawInputBuffer( reinterpret_cast<PRAWINPUT>( &vRi[ 0 ] ), &cbSize2, sizeof( RAWINPUTHEADER ) );
            if ( uRiRead == static_cast<UINT>( -1 ) )
            {
                B33_LOG( B33::Core::Debug::Error, L"GetRawInputBuffer error %d", GetLastError() );
                return;
            }

            pWd->Data.LastEvent |= EAbWindowEvents::Input;
            is.Event = EAbInputEvents::AbMotion;
            pRi      = reinterpret_cast<PRAWINPUT>( &vRi[ 0 ] );
            for ( size_t i = 0; i < uRiRead; ++i, pRi = NEXTRAWINPUTBLOCK( pRi ) )
            {
                auto &mouse = pRi->data.mouse;

                if ( pRi->header.dwType != RIM_TYPEMOUSE || mouse.usFlags & MOUSE_MOVE_ABSOLUTE )
                    continue;

                is.Mouse.MouseX += mouse.lLastX;
                is.Mouse.MouseY += mouse.lLastY;
            }
            pWd->Data.InputStruct.push( is );

            return;
        }
    }

    return BasicWin32WindowPolicy::OnUpdate( uMsg, wParam, lParam );
}

} // namespace B33::App
#endif // !_WIN32
