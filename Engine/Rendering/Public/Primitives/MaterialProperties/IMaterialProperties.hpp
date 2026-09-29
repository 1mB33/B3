#if !defined( B33_IMATERIAL_PROPERTIES_HPP )
#    define B33_IMATERIAL_PROPERTIES_HPP

namespace B33::Rendering
{

class alignas( 4 ) IMaterialProperties
{
  public:
    IMaterialProperties() = default;

    ~IMaterialProperties() = default;

  public:
    IMaterialProperties( const IMaterialProperties & ) noexcept            = default;
    IMaterialProperties &operator=( const IMaterialProperties & ) noexcept = default;

    IMaterialProperties( IMaterialProperties && ) noexcept            = default;
    IMaterialProperties &operator=( IMaterialProperties && ) noexcept = default;
};

} // namespace B33::Rendering
#endif // !B33_IMATERIAL_PROPERTIESP_HPP
