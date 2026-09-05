#if !defined( B33_MINIMAL_HARDWARE_HPP )
#    define B33_MINIMAL_HARDWARE_HPP

#    include "Vulkan/IHardware.hpp"

namespace B33::Rendering
{

class MinimalHardware : public IHardware<MinimalHardware>
{
  public:
    __B33_API ::VkPhysicalDevice ChooseHardwareImpl( VkInstance Instance ) const;
};

} // namespace B33::Rendering
#endif // !B33_MINIMAL_HARDWARE_HPP
