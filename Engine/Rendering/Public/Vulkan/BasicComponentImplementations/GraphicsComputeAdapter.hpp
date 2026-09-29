#if !defined( B33_COMPUTE_DEVICE_ADAPTER_HPP )
#    define B33_COMPUTE_DEVICE_ADAPTER_HPP

#    include <B33Core.h>
#    include "Vulkan/IAdapter.hpp"

namespace B33::Rendering
{

class GraphicsComputeAdapter : public IAdapter<GraphicsComputeAdapter>
{
    template <typename T>
    using Vector = ::std::vector<T>;

    // Interface Implementation // ------------------------------------------------------------------------------------
  public:
    __B33_API const Vector<const char *> &GetExtensionsImpl() const;
    __B33_API void                       *GetFeaturesImpl() const;
    __B33_API u32                         GetQueueFlagsImpl() const;

  private:
    Vector<const char *> m_vExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};

} // namespace B33::Rendering
#endif // !B33_COMPUTE_DEVICE_ADAPTER_HPP
