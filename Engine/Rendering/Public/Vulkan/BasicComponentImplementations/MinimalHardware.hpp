#ifndef B33_MINIMAL_HARDWARE_H
#define B33_MINIMAL_HARDWARE_H

#include "Vulkan/Instance.hpp"
#include "Vulkan/IHardware.hpp"

namespace B33::Rendering
{

class MinimalHardware : public ::B33::Rendering::IHardware<MinimalHardware>
{
  public:
    __B33_API ::VkPhysicalDevice ChooseHardwareImpl( VkInstance Instance ) const;
};

} // namespace B33::Rendering
#endif // !B33_MINIMAL_HARDWARE_H
