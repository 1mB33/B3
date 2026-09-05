#include "B33Core.h"

#include "TerminalColors.hpp"

namespace B33::Core
{

using namespace std;

// --------------------------------------------------------------------------------------------------------------------
ColorizeTerminal::String ColorizeTerminal::Colorize( ColorizeTerminal::String strText, EColors color )
{
    switch ( color )
    {
        case BrightBlack:
#if defined( __APPLE__ ) || defined( __linux__ )
            return "\033[90m" + strText + "\033[0m";
#endif
        case BrightGreen:
#if defined( __APPLE__ ) || defined( __linux__ )
            return "\033[92m" + strText + "\033[0m";
#endif
        case BrightYellow:
#if defined( __APPLE__ ) || defined( __linux__ )
            return "\033[93m" + strText + "\033[0m";
#endif
        case BrightRed:
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
ColorizeTerminal::WString ColorizeTerminal::Colorize( ColorizeTerminal::WString wstrText, EColors color )
{
    switch ( color )
    {
        case BrightBlack:
#if defined( __APPLE__ ) || defined( __linux__ )
            return L"\033[90m" + wstrText + L"\033[0m";
#endif
        case BrightGreen:
#if defined( __APPLE__ ) || defined( __linux__ )
            return L"\033[92m" + wstrText + L"\033[0m";
#endif
        case BrightYellow:
#if defined( __APPLE__ ) || defined( __linux__ )
            return L"\033[93m" + wstrText + L"\033[0m";
#endif
        case BrightRed:
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
