#if !defined(B33_PURE_GRAPHICS_DEVICE_ADAPTER_H)
#define B33_PURE_GRAPHICS_DEVICE_ADAPTER_H

#include "Vulkan/IAdapter.hpp"

namespace B33::Rendering
{

class PureGraphicsAdapter : public ::B33::Rendering::IAdapter<::B33::Rendering::PureGraphicsAdapter>
{
    // Interface Implementation // ------------------------------------------------------------------------------------
  public:
    __B33_API const ::std::vector<const char *> &GetExtensionsImpl() const;
    __B33_API void                              *GetFeaturesImpl() const;
    __B33_API uint32_t                           GetQueueFlagsImpl() const;

  private:
    ::std::vector<const char *> m_vExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                                                  VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME };
};

} // namespace B33::Rendering
#endif // !B33_PURE_GRAPHICS_DEVICE_ADAPTER_H
