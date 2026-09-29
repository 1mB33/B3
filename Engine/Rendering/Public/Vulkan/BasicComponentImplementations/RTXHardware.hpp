#if !defined( B33_RTX_HARDWARE_HPP )
#    define B33_RTX_HARDWARE_HPP

#    include "Vulkan/IHardware.hpp"

namespace B33::Rendering
{

/**
 * Hardware wrapper that selects a GPU with features:
 * - rayTracingPipeline
 * - accelerationStructure
 * - bufferDeviceAddress
 **/
class RTXHardware : public IHardware<RTXHardware>
{
    // Interface Implementation // ------------------------------------------------------------------------------------
  private:
    VkPhysicalDevice ChooseHardwareImpl( VkInstance Instance );
};

} // namespace B33::Rendering
#endif // !B33_RTX_HARDWARE_HPP
