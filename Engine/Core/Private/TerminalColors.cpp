#include "B33Core.h"

#include "TerminalColors.hpp"

namespace B33::Core
{

// --------------------------------------------------------------------------------------------------------------------
::std::string ColorizeTerminal::Colorize( ::std::string strText, EColors color )
{
    switch ( color )
    {
        case BrigthBlack:
#if defined( __APPLE__ ) || defined( __linux__ )
            return "\033[90m" + strText + "\033[0m";
#endif
        case BrigthGreen:
#if defined( __APPLE__ ) || defined( __linux__ )
            return "\033[92m" + strText + "\033[0m";
#endif
        case BrigthYellow:
#if defined( __APPLE__ ) || defined( __linux__ )
            return "\033[93m" + strText + "\033[0m";
#endif
        case BrigthRed:
#if defined( __APPLE__ ) || defined( __linux__ )
            return "\033[91m" + strText + "\033[0m";
#endif
        case DarkGreen:
#if defined( __APPLE__ ) || defined( __linux__ )
            return "\033[32m" + strText + "\033[0m";
#endif
        default:
            return strText;
    };
}

// --------------------------------------------------------------------------------------------------------------------
::std::wstring ColorizeTerminal::Colorize( ::std::wstring wstrText, EColors color )
{
    switch ( color )
    {
        case BrigthBlack:
#if defined( __APPLE__ ) || defined( __linux__ )
            return L"\033[90m" + wstrText + L"\033[0m";
#endif
        case BrigthGreen:
#if defined( __APPLE__ ) || defined( __linux__ )
            return L"\033[92m" + wstrText + L"\033[0m";
#endif
        case BrigthYellow:
#if defined( __APPLE__ ) || defined( __linux__ )
            return L"\033[93m" + wstrText + L"\033[0m";
#endif
        case BrigthRed:
#if defined( __APPLE__ ) || defined( __linux__ )
            return L"\033[91m" + wstrText + L"\033[0m";
#endif
        case DarkGreen:
#if defined( __APPLE__ ) || defined( __linux__ )
            return L"\033[32m" + wstrText + L"\033[0m";
#endif
        default:
            return wstrText;
    };
}

} // namespace B33::Core
