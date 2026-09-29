#if !defined( B33_ROUGHNESS_PROPERTY_HPP )
#    define B33_ROUGHNESS_PROPERTY_HPP

namespace B33::Rendering
{

// FIXME: Some compilers add one byte in the empty IMaterialProperties class
class alignas( 8 ) RoughnessProperty // : public IMaterialProperties
{
    template <typename T>
    using Vector = ::std::vector<T>;

  public:
    RoughnessProperty()
      : m_fRoughness( {} )
    {
    }

  public:
    void SetReflection( float fRef, usize uIndex )
    {
        if ( fRef >= 0.25f )
        {
            m_fRoughness[ uIndex ] = 0.25f;
            return;
        }
        m_fRoughness[ uIndex ] = fRef * 0.25f;
    }

  private:
    Vector<float> m_fRoughness;
};

} // namespace B33::Rendering
#endif // !B33_ROUGHNESS_PROPERTY_HPP
