#if !defined( B33_COLOR_PROPERTY_HPP )
#    define B33_COLOR_PROPERTY_HPP

namespace B33::Rendering
{

// FIXME: Some compilers add one byte in the empty IMaterialProperties class
class alignas( 8 ) ColorProperty // : public IMaterialProperties
{
    template <typename T>
    using Vector = ::std::vector<T>;

  public:
    ColorProperty()
      : m_uColors( {} )
    {
    }

  public:
    void SetColor( u32 uColor, usize uIndex )
    {
        m_uColors[ uIndex ] = ( uColor & 0xFFFFFF00 ) | ( m_uColors[ uIndex ] & 0x000000FF );
    }

    void SetAlpha( u32 uAlpha, usize uIndex )
    {
        m_uColors[ uIndex ] = ( uAlpha & 0x000000FF ) | ( m_uColors[ uIndex ] & 0xFFFFFF00 );
    }

    void SetColorAndAlpha( u32 uHexValue, usize uIndex )
    {
        m_uColors[ uIndex ] = uHexValue;
    }

  private:
    Vector<u32> m_uColors;
};

} // namespace B33::Rendering
#endif // !B33_COLOR_PROPERTY_HPP
