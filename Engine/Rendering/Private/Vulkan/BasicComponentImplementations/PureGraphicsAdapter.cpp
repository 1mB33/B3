#include "B33Rendering.hpp"

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
    return NULL;
}

// --------------------------------------------------------------------------------------------------------------------
uint32_t PureGraphicsAdapter::GetQueueFlagsImpl() const
{
    return ::VK_QUEUE_GRAPHICS_BIT;
}

}; // namespace B33::Rendering
