#if !defined( B33_COLORED_CUBE_HPP )
#    define B33_COLORED_CUBE_HPP

#    include "Primitives/Cube.hpp"
#    include "Primitives/MaterialProperties/ColorProperty.hpp"
#    include "Primitives/MaterialProperties/ReflectionProperty.hpp"
#    include "Primitives/MaterialProperties/RoughnessProperty.hpp"

namespace B33::Rendering
{

class alignas( 16 ) ColoredCube
  : public ::B33::Math::Cube
  , public ColorProperty
  , public ReflectionProperty
  , public RoughnessProperty
{
    __B33_ATTRIBUTE_MIGHT_BE_UNUSED u32 m_uPadding;
};

} // namespace B33::Rendering
#endif // !B33_COLORED_CUBE_HPP
