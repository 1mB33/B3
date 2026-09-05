#include "B33Rendering.h"

#include "Vulkan/BasicComponentImplementations/PureGraphicsAdapter.hpp"

namespace B33::Rendering
{

using namespace std;

// Public // ----------------------------------------------------------------------------------------------------------
const vector<const char *> &PureGraphicsAdapter::GetExtensionsImpl() const
{
    return m_vExtensions;
}

// --------------------------------------------------------------------------------------------------------------------
void *PureGraphicsAdapter::GetFeaturesImpl() const
{
    static VkPhysicalDeviceVulkan12Features indexFeatures = {};
    indexFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    indexFeatures.descriptorBindingStorageBufferUpdateAfterBind = VK_TRUE;

    static VkPhysicalDeviceVulkan13Features dynRen = {};
    dynRen.sType                                   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    dynRen.pNext                                   = &indexFeatures;
    dynRen.dynamicRendering                        = VK_TRUE;

    return &dynRen;
}

// --------------------------------------------------------------------------------------------------------------------
u32 PureGraphicsAdapter::GetQueueFlagsImpl() const
{
    return ::VK_QUEUE_GRAPHICS_BIT;
}

}; // namespace B33::Rendering
