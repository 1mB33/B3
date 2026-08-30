#ifndef B33_WRAPPER_ADAPTER_H
#define B33_WRAPPER_ADAPTER_H

#include "Vulkan/WrapperHardware.hpp"

namespace B33::Rendering
{

class AdapterWrapper
{
  public:
    __B33_API AdapterWrapper();

    __B33_API ~AdapterWrapper();

  public:
    AdapterWrapper( AdapterWrapper && ) noexcept = default;
    AdapterWrapper( const AdapterWrapper & )     = delete;

    AdapterWrapper &operator=( AdapterWrapper && )      = delete;
    AdapterWrapper &operator=( const AdapterWrapper & ) = delete;

    // Getters // -----------------------------------------------------------------------------------------------------
  public:
    uint32_t GetQueueFamilyIndex() const
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

    size_t GetPushConstantsLimit() const
    {
        return m_PushConstantsLimit;
    }

    // Methods // -----------------------------------------------------------------------------------------------------
  public:
    template <class T>
    void Initialize( ::std::weak_ptr<const ::B33::Rendering::HardwareWrapper> pHardware, const T &adapter )
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
    __B33_API uint32_t ChooseQueueFamily( VkPhysicalDevice gpu, const uint32_t uFlags ) const;

    __B33_API VkDevice CreateDevice( VkPhysicalDevice                 gpu,
                                     const std::vector<const char *> &vExtensions,
                                     const void                      *pFeatures,
                                     const uint32_t                   uFamilyIndex ) const;

    __B33_API VkQueue CreateQueue( VkDevice dv, uint32_t uQueueIndex ) const;

  private:
    uint32_t m_uFlags       = 0;
    uint32_t m_uQueueFamily = 0;

    VkDevice m_Device = VK_NULL_HANDLE;
    VkQueue  m_Queue  = VK_NULL_HANDLE;

    size_t m_PushConstantsLimit = -1;
};

} // namespace B33::Rendering
#endif // !B33_WRAPPER_ADAPTER_H
