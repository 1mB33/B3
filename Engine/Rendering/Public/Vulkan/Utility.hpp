#if !defined( B33_UTILITY_HPP )
#    define B33_UTILITY_HPP

#    include "Vulkan/WrapperAdapter.hpp"

namespace B33::Rendering
{

class Shaders
{
  public:
    __B33_API static ::VkShaderModule LoadShader( const ::std::string                    &strPath,
                                                  const ::B33::Rendering::AdapterWrapper *pAdapter );
};

}; // namespace B33::Rendering

#endif
