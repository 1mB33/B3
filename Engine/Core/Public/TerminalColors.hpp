#if !defined( B33_TERMINAL_COLORS )
#    define B33_TERMINAL_COLORS

namespace B33::Core
{

enum EColors
{
    BrigthBlack,
    BrigthGreen,
    BrigthYellow,
    BrigthRed,

    DarkGreen,
    DarkYellow,
    DarkRed
};

class ColorizeTerminal
{
  public:
    ColorizeTerminal()  = default;
    ~ColorizeTerminal() = default;

  public:
    __B33_API static ::std::string Colorize( ::std::string strText, EColors color );

    __B33_API static ::std::wstring Colorize( ::std::wstring wstrText, EColors color );
};

} // namespace B33::Core

#endif
