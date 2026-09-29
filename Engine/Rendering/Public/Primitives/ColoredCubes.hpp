#if !defined( B33_COLORED_CUBES_HPP )
#    define B33_COLORED_CUBES_HPP

#    include "Primitives/Cubes.hpp"
#    include "Primitives/MaterialProperties/ColorProperty.hpp"
#    include "Primitives/MaterialProperties/ReflectionProperty.hpp"
#    include "Primitives/MaterialProperties/RoughnessProperty.hpp"

namespace B33::Rendering
{

class ColoredCubes
  : public ::B33::Math::Cubes
  , public ColorProperty
  , public ReflectionProperty
  , public RoughnessProperty
{
};

} // namespace B33::Rendering
#endif // !B33_COLORED_CUBES_HPP
