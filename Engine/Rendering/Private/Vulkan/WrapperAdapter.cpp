#include "B33Rendering.h"

#include "Vulkan/ErrorHandling.hpp"
#include "Vulkan/WrapperAdapter.hpp"

namespace B33::Rendering
{

using namespace std;

// Constructors // ----------------------------------------------------------------------------------------------------
AdapterWrapper::AdapterWrapper()
  : m_uFlags( -1 )
  , m_uQueueFamily()
  , m_Device()
  , m_Queue()
  , m_PushConstantsLimit( 0 )
{
}

// --------------------------------------------------------------------------------------------------------------------
AdapterWrapper::~AdapterWrapper() noexcept
{
    if ( m_Device != VK_NULL_HANDLE )
    {
        B33_LOG( Core::Debug::Info, L"Destroying adapter" );
        vkDestroyDevice( m_Device, NULL );
        m_Device = VK_NULL_HANDLE;
    }
}

// Private // ---------------------------------------------------------------------------------------------------------
u32 AdapterWrapper::ChooseQueueFamily( VkPhysicalDevice gpu, const u32 uFlags ) const
{
    B33_TRACE( L"AdapterWrapper::ChooseQueueFamily()" );
    u32                             uFamilyCount;
    vector<VkQueueFamilyProperties> vProperties    = {};
    VkPhysicalDevice                physicalDevice = gpu;

    vkGetPhysicalDeviceQueueFamilyProperties( physicalDevice, &uFamilyCount, NULL );
    if ( !uFamilyCount )
    {
        B33_LOG( Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!!" );
        throw B33_EXCEPT( "Ohh nooo... Vulkan isn't working!!!" );
    }
    vProperties.resize( uFamilyCount );

    vkGetPhysicalDeviceQueueFamilyProperties( physicalDevice, &uFamilyCount, &vProperties[ 0 ] );
    if ( !uFamilyCount )
    {
        B33_LOG( Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!!" );
        throw B33_EXCEPT( "Ohh nooo... Vulkan isn't working!!!" );
    }

    for ( u32 i = 0; i < uFamilyCount; ++i )
        if ( vProperties[ i ].queueFlags & uFlags )
            return i;

    B33_LOG( Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!!" );
    throw B33_EXCEPT( "Ohh nooo... Vulkan isn't working!!!" );
}

// --------------------------------------------------------------------------------------------------------------------
VkDevice AdapterWrapper::CreateDevice( VkPhysicalDevice            gpu,
                                       const Vector<const char *> &vExtensions,
                                       const void                 *pFeatures,
                                       const u32                   uFamilyIndex ) const
{
    B33_TRACE( L"AdapterWrapper::CreateDevice()" );
    VkDevice device            = VK_NULL_HANDLE;
    float    queuePriorities[] = { 1. };

    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex        = uFamilyIndex;
    queueCreateInfo.queueCount              = size( queuePriorities );
    queueCreateInfo.pQueuePriorities        = queuePriorities;

    VkDeviceCreateInfo createInfo      = {};
    createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext                   = pFeatures;
    createInfo.queueCreateInfoCount    = 1;
    createInfo.pQueueCreateInfos       = &queueCreateInfo;
    createInfo.enabledExtensionCount   = static_cast<u32>( !vExtensions.empty() ? vExtensions.size() : 0 );
    createInfo.ppEnabledExtensionNames = !vExtensions.empty() ? &vExtensions[ 0 ] : NULL;
    createInfo.pEnabledFeatures        = NULL;

    THROW_IF_FAILED( vkCreateDevice( gpu, &createInfo, NULL, &device ) );

    return device;
}

// --------------------------------------------------------------------------------------------------------------------
VkQueue AdapterWrapper::CreateQueue( VkDevice dv, const u32 uQueueIndex ) const
{
    B33_TRACE( L"AdapterWrapper::CreateQueue()" );
    VkQueue graphicsQueue;

    vkGetDeviceQueue( dv, uQueueIndex, 0, &graphicsQueue );

    return graphicsQueue;
}

} // namespace B33::Rendering
