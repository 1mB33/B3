#include "B33Rendering.hpp"

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
    return NULL;
}

// --------------------------------------------------------------------------------------------------------------------
uint32_t GraphicsComputeAdapter::GetQueueFlagsImpl() const
{
    return ::VK_QUEUE_COMPUTE_BIT;
}

}; // namespace B33::Rendering
