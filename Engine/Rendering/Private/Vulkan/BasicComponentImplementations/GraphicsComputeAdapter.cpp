#include "B33Rendering.h"

#include "Vulkan/BasicComponentImplementations/GraphicsComputeAdapter.hpp"

namespace B33::Rendering
{

using namespace std;

// Public // ----------------------------------------------------------------------------------------------------------
const vector<const char *> &GraphicsComputeAdapter::GetExtensionsImpl() const
{
    return m_vExtensions;
}

// --------------------------------------------------------------------------------------------------------------------
void *GraphicsComputeAdapter::GetFeaturesImpl() const
{
    static VkPhysicalDeviceVulkan12Features indexFeatures       = {};
    indexFeatures.sType                                         = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    indexFeatures.descriptorBindingStorageBufferUpdateAfterBind = VK_TRUE;

    return &indexFeatures;
}

// --------------------------------------------------------------------------------------------------------------------
u32 GraphicsComputeAdapter::GetQueueFlagsImpl() const
{
    return ::VK_QUEUE_COMPUTE_BIT;
}

}; // namespace B33::Rendering
