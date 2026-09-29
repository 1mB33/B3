#if !defined( B33_WRAPPER_ADAPTER_HPP )
#    define B33_WRAPPER_ADAPTER_HPP

#    include "Vulkan/WrapperHardware.hpp"

namespace B33::Rendering
{

class AdapterWrapper
{
    template <typename T>
    using Vector = ::std::vector<T>;
    template <typename T>
    using WeakPtr = ::std::weak_ptr<T>;

  public:
    __B33_API AdapterWrapper();

    __B33_API ~AdapterWrapper() noexcept;

  public:
    AdapterWrapper( AdapterWrapper && ) noexcept = default;
    AdapterWrapper( const AdapterWrapper & )     = delete;

    AdapterWrapper &operator=( AdapterWrapper && )      = delete;
    AdapterWrapper &operator=( const AdapterWrapper & ) = delete;

    // Getters // -----------------------------------------------------------------------------------------------------
  public:
    u32 GetQueueFamilyIndex() const
    {
        return m_uQueueFamily;
    }

    VkDevice GetAdapterHandle() const
    {
        return m_Device;
    }

    VkQueue GetQueueHandle() const
    {
        return m_Queue;
    }

    usize GetPushConstantsLimit() const
    {
        return m_PushConstantsLimit;
    }

    // Methods // -----------------------------------------------------------------------------------------------------
  public:
    template <class T>
    void Initialize( WeakPtr<const HardwareWrapper> pHardware, const T &adapter )
    {
        B33_LOG( Core::Debug::Info, L"Initializing adapter" );
        auto pLockedHardware = pHardware.lock();

        if ( !pLockedHardware )
            throw B33_EXCEPT( "Cannot lock hardware on adapter initialization" );

        m_uFlags       = adapter.GetQueueFlags();
        m_uQueueFamily = ChooseQueueFamily( pLockedHardware->GetPhysicalDevice(), m_uFlags );
        m_Device       = CreateDevice( pLockedHardware->GetPhysicalDevice(),
                                       adapter.GetExtensions(),
                                       adapter.GetFeatures(),
                                       m_uQueueFamily );
        m_Queue        = CreateQueue( m_Device, m_uQueueFamily );

        VkPhysicalDeviceProperties pProperties;
        vkGetPhysicalDeviceProperties( pLockedHardware->GetPhysicalDevice(), &pProperties );

        m_PushConstantsLimit = pProperties.limits.maxPushConstantsSize;
    }

    // Internal // ----------------------------------------------------------------------------------------------------
  private:
    __B33_API u32 ChooseQueueFamily( VkPhysicalDevice gpu, const u32 uFlags ) const;

    __B33_API VkDevice CreateDevice( VkPhysicalDevice            gpu,
                                     const Vector<const char *> &vExtensions,
                                     const void                 *pFeatures,
                                     const u32                   uFamilyIndex ) const;

    __B33_API VkQueue CreateQueue( VkDevice dv, u32 uQueueIndex ) const;

  private:
    u32 m_uFlags       = 0;
    u32 m_uQueueFamily = 0;

    VkDevice m_Device = VK_NULL_HANDLE;
    VkQueue  m_Queue  = VK_NULL_HANDLE;

    usize m_PushConstantsLimit = -1;
};

} // namespace B33::Rendering
#endif // !B33_WRAPPER_ADAPTER_HPP
