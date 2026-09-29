#if !defined( B33_UTILITY_HPP )
#    define B33_UTILITY_HPP

#    include "Vulkan/WrapperAdapter.hpp"

namespace B33::Rendering
{

class Shaders
{
    using String = ::std::string;

  public:
    __B33_API static ::VkShaderModule LoadShader( const String &strPath, const AdapterWrapper *pAdapter );
};

}; // namespace B33::Rendering

#endif
