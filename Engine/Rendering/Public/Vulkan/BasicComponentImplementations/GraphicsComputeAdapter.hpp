#ifndef B33_COMPUTE_DEVICE_ADAPTER_H
#define B33_COMPUTE_DEVICE_ADAPTER_H

#include "Vulkan/IAdapter.hpp"

namespace B33::Rendering
{

class GraphicsComputeAdapter : public ::B33::Rendering::IAdapter<::B33::Rendering::GraphicsComputeAdapter>
{
    // Interface Implementation // ------------------------------------------------------------------------------------
  public:
    const ::std::vector<const char *> &GetExtensionsImpl() const;
    void                              *GetFeaturesImpl() const;
    uint32_t                           GetQueueFlagsImpl() const;

  private:
    ::std::vector<const char *> m_vExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};

} // namespace B33::Rendering
#endif // !B33_COMPUTE_DEVICE_ADAPTER_H
