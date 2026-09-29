#if !defined( B33_REFLECTION_PROPERTY_HPP )
#    define B33_REFLECTION_PROPERTY_HPP

namespace B33::Rendering
{

// FIXME: Some compilers add one byte in the empty IMaterialProperties class
class alignas( 8 ) ReflectionProperty //: public IMaterialProperties
{
    template <typename T>
    using Vector = ::std::vector<T>;

  public:
    ReflectionProperty()
      : m_fReflections( {} )
    {
    }

  public:
    void SetReflection( float fRef, usize uIndex )
    {
        if ( fRef >= 0.5f )
        {
            m_fReflections[ uIndex ] = 0.5f;
            return;
        }
        m_fReflections[ uIndex ] = fRef * 0.5f;
    }

  private:
    Vector<float> m_fReflections;
};

} // namespace B33::Rendering
#endif // !B33_REFLECTION_PROPERTY_HPP
