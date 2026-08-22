#ifndef B33_COMPUTE_DEVICE_ADAPTER_H
#define B33_COMPUTE_DEVICE_ADAPTER_H

#include "ExportImport.h"
#include "Vulkan/IAdapter.hpp"

namespace B33::Rendering
{

class GraphicsComputeAdapter : public ::B33::Rendering::IAdapter<::B33::Rendering::GraphicsComputeAdapter>
{
    // Interface Implementation // ------------------------------------------------------------------------------------
  public:
    __B33_API const ::std::vector<const char *> &GetExtensionsImpl() const;
    __B33_API void                              *GetFeaturesImpl() const;
    __B33_API uint32_t                           GetQueueFlagsImpl() const;

  private:
    ::std::vector<const char *> m_vExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};

} // namespace B33::Rendering
#endif // !B33_COMPUTE_DEVICE_ADAPTER_H
