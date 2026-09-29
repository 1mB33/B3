#if !defined( B33_TERMINAL_COLORS_HPP )
#    define B33_TERMINAL_COLORS_HPP

namespace B33::Core
{

enum EColors
{
    BrightBlack,
    BrightGreen,
    BrightYellow,
    BrightRed,

    DarkGreen,
    DarkYellow,
    DarkRed
};

class ColorizeTerminal
{
    using String  = ::std::string;
    using WString = ::std::wstring;

  public:
    __B33_API static String Colorize( String strText, EColors color );

    __B33_API static WString Colorize( WString wstrText, EColors color );
};

} // namespace B33::Core

#endif
