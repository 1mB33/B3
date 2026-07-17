#ifdef _WIN32

#    include "Window/BaseWindowDetails.h"
#    include "Window/WindowPolicy/Win32/BasicWin32Policy.hpp"
#    include <windowsx.h>

namespace B33::App
{

using namespace B33::Core;

// ---------------------------------------------------------------------------------------------------------------------
uint32_t BasicWin32WindowPolicy::CreateImpl( WindowDesc *pWd )
{
    B33_ASSERT( pWd );

    auto &windowData = pWd->Data;
    auto &windowOS   = pWd->OS;


    B33_ASSERT( windowOS.hWnd == NULL );
    B33_ASSERT( !windowData.bIsAlive );
    B33_ASSERT( windowData.Height > 0 );
    B33_ASSERT( windowData.Width > 0 );

    HWND hWnd;

    m_pWindowDesc = pWd;

    this->OnPreWcex();

    // Fallback to default class name if none is provided,
    // if pwszClassName is provided, we assume that WCEX is already filled
    if ( windowData.pwszClassName == NULL || wcscmp( windowData.pwszClassName, L"" ) == 0 )
    {
        windowData.pwszClassName = L"AtlanticClass";

        memset( &pWd->OS.Wcex, 0, sizeof( WNDCLASSEX ) );

        pWd->OS.Wcex.cbSize        = sizeof( WNDCLASSEX );
        pWd->OS.Wcex.style         = CS_HREDRAW | CS_VREDRAW;
        pWd->OS.Wcex.hInstance     = GetModuleHandle( NULL );
        pWd->OS.Wcex.hCursor       = LoadCursor( NULL, IDC_ARROW );
        pWd->OS.Wcex.lpszClassName = windowData.pwszClassName;
        pWd->OS.Wcex.lpfnWndProc   = BasicWin32WindowPolicy::WindowProc;
    }

    AbAskToRegisterWindowClass( windowData.pwszClassName, windowOS.Wcex );

    this->OnPreRegister();

    if ( windowOS.hWnd == NULL )
    {
        hWnd = CreateWindowEx( WS_EX_OVERLAPPEDWINDOW,
                               windowData.pwszClassName,
                               windowData.Name.c_str(),
                               WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT,
                               CW_USEDEFAULT,
                               windowData.Width,
                               windowData.Height,
                               NULL,
                               NULL,
                               GetModuleHandle( NULL ),
                               this );

        if ( hWnd == NULL )
        {
            B33_LOG( B33::Core::Debug::Error, L"Couldn't CreateWindow(), last error %u", GetLastError() );
            return -1;
        }

        windowOS.hWnd = hWnd;
    }

    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicWin32WindowPolicy::ShowImpl( WindowDesc *pWd )
{
    B33_ASSERT( pWd );

    auto                                       &windowOS   = pWd->OS;
    __B33_ATTRIBUTE_MIGHT_BE_UNUSED const auto &windowData = pWd->Data;


    B33_ASSERT( windowData.bIsAlive );
    B33_ASSERT( windowOS.hWnd != NULL );

    ShowWindow( windowOS.hWnd, SW_SHOW );
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicWin32WindowPolicy::HideImpl( WindowDesc *pWd )
{
    B33_ASSERT( pWd );

    auto                                       &windowOS   = pWd->OS;
    __B33_ATTRIBUTE_MIGHT_BE_UNUSED const auto &windowData = pWd->Data;


    B33_ASSERT( windowData.bIsAlive );
    B33_ASSERT( windowOS.hWnd != NULL );

    ShowWindow( windowOS.hWnd, SW_HIDE );
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicWin32WindowPolicy::DestroyImpl( WindowDesc *pWd )
{
    B33_ASSERT( pWd );

    auto       &windowOS   = pWd->OS;
    const auto &windowData = pWd->Data;


    B33_ASSERT( windowData.bIsAlive );
    B33_ASSERT( windowOS.hWnd != NULL );

    if ( DestroyWindow( windowOS.hWnd ) )
    {
        windowOS.hWnd = NULL;
    }

    AbAskToCloseWindowClass( windowData.pwszClassName );
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicWin32WindowPolicy::UpdateImpl( WindowDesc *pWd )
{
    B33_ASSERT( pWd );

    auto                                       &windowOS   = pWd->OS;
    __B33_ATTRIBUTE_MIGHT_BE_UNUSED const auto &windowData = pWd->Data;


    B33_ASSERT( windowData.bIsAlive );


    MSG msg;
    while ( PeekMessage( &msg, windowOS.hWnd, 0, 0, PM_REMOVE ) != 0 )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicWin32WindowPolicy::OnUpdate( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( uMsg )
    {
        case WM_KEYDOWN:
        {
            // UserInput class handles continuos press by itself, no need to spam
            WORD wKeyFlags = HIWORD( lParam );
            if ( wKeyFlags & KF_REPEAT )
            {
                return;
            }

            AbInputStruct is = {};
            m_pWindowDesc->Data.LastEvent |= EAbWindowEvents::Input;

            is.Event          = EAbInputEvents::AbKeyPress;
            is.Keyboard.KeyId = LOWORD( wKeyFlags );

            m_pWindowDesc->Data.InputStruct.push( is );
            return;
        }

        case WM_KEYUP:
        {
            AbInputStruct is = {};
            m_pWindowDesc->Data.LastEvent |= EAbWindowEvents::Input;

            is.Event          = EAbInputEvents::AbKeyRelease;
            is.Keyboard.KeyId = LOWORD( HIWORD( lParam ) );

            m_pWindowDesc->Data.InputStruct.push( is );
            return;
        }

        case WM_LBUTTONDOWN:
        {
            AbInputStruct is = {};
            m_pWindowDesc->Data.LastEvent |= EAbWindowEvents::Input;

            is.Event             = EAbInputEvents::AbButtonPress;
            is.MouseButton.KeyId = 1;

            m_pWindowDesc->Data.InputStruct.push( is );
            break;
        }

        case WM_RBUTTONDOWN:
        {
            AbInputStruct is = {};
            m_pWindowDesc->Data.LastEvent |= EAbWindowEvents::Input;

            is.Event             = EAbInputEvents::AbButtonPress;
            is.MouseButton.KeyId = 3;

            m_pWindowDesc->Data.InputStruct.push( is );
            break;
        }

        case WM_MBUTTONDOWN:
        {
            AbInputStruct is = {};
            m_pWindowDesc->Data.LastEvent |= EAbWindowEvents::Input;

            is.Event             = EAbInputEvents::AbButtonPress;
            is.MouseButton.KeyId = 2;

            m_pWindowDesc->Data.InputStruct.push( is );
            break;
        }

        case WM_MOUSEMOVE:
        {
            AbInputStruct is = {};
            m_pWindowDesc->Data.LastEvent |= EAbWindowEvents::Input;

            is.Event        = EAbInputEvents::AbMotion;
            is.Mouse.MouseX = GET_X_LPARAM( lParam );
            is.Mouse.MouseY = GET_Y_LPARAM( lParam );

            m_pWindowDesc->Data.InputStruct.push( is );

            return;
        }

        case WM_SIZE:
            m_pWindowDesc->Data.Width  = LOWORD( lParam );
            m_pWindowDesc->Data.Height = HIWORD( lParam );
            m_pWindowDesc->Data.LastEvent |= EAbWindowEvents::Resize;
            break;

        case WM_CLOSE:
            m_pWindowDesc->Data.LastEvent |= EAbWindowEvents::Destroy;
            break;

        default:
            break;
    }
}

} // namespace B33::App
#endif // !_WIN32
