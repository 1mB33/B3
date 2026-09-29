#if !defined( B33_DEVICE_ADAPTER_HPP )
#    define B33_DEVICE_ADAPTER_HPP

#    include "Vulkan/IAdapter.hpp"

namespace B33::Rendering
{

class RTXDeviceAdapter : public IAdapter<RTXDeviceAdapter>
{
    template <typename T>
    using Vector = ::std::vector<T>;

    // Interface implementation // ------------------------------------------------------------------------------------
  public:
    __B33_API const Vector<const char *> GetExtensionsImpl() const;

    __B33_API void *GetFeaturesImpl() const;

    __B33_API u32 GetQueueFlagsImpl() const;
};

} // namespace B33::Rendering
#endif // !B33_DEVICE_ADAPTER_HPP
