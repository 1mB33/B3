#if !defined(B33_RTX_HARDWARE_H)
#define B33_RTX_HARDWARE_H

#include "Vulkan/Instance.hpp"
#include "Vulkan/IHardware.hpp"

namespace B33::Rendering
{

/**
 * Hardware wrapper that selects a GPU with features:
 * - rayTracingPipeline
 * - accelerationStructure
 * - bufferDeviceAddress
 **/
class RTXHardware : public ::B33::Rendering::IHardware<RTXHardware>
{
    // Interface Implementation // ------------------------------------------------------------------------------------
  private:
    VkPhysicalDevice ChooseHardwareImpl( VkInstance Instance );
};

} // namespace B33::Rendering
#endif // !B33_RTX_HARDWARE_H
