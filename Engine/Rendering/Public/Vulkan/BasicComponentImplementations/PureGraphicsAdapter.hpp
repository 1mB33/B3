#if !defined( B33_PURE_GRAPHICS_DEVICE_ADAPTER_HPP )
#    define B33_PURE_GRAPHICS_DEVICE_ADAPTER_HPP

#    include "Vulkan/IAdapter.hpp"

namespace B33::Rendering
{

class PureGraphicsAdapter : public IAdapter<PureGraphicsAdapter>
{
    template <typename T>
    using Vector = ::std::vector<T>;

    // Interface Implementation // ------------------------------------------------------------------------------------
  public:
    __B33_API const Vector<const char *> &GetExtensionsImpl() const;
    __B33_API void                       *GetFeaturesImpl() const;
    __B33_API u32                         GetQueueFlagsImpl() const;

  private:
    Vector<const char *> m_vExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME };
};

} // namespace B33::Rendering
#endif // !B33_PURE_GRAPHICS_DEVICE_ADAPTER_HPP
