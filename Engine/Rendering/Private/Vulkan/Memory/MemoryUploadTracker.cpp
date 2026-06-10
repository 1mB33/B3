#include "B33Rendering.hpp"

#include "Vulkan/ErrorHandling.hpp"
#include "Vulkan/Memory/MemoryUploadTracker.hpp"

namespace B33::Rendering
{

using namespace std;
using namespace B33::Core;
using namespace B33::Core::Debug;

// Constructors // ----------------------------------------------------------------------------------------------------
MemoryUploadTracker::MemoryUploadTracker()
  : m_Reupload( RequestStaging )
{
}

// Public // ----------------------------------------------------------------------------------------------------------
EReupload MemoryUploadTracker::ReuploadStatus()
{
    switch ( m_Reupload )
    {
        case EReupload::NoAction:
            return EReupload::NoAction;
        case EReupload::RequestStaging:
            m_Reupload = EReupload::RequestGpuUpload;
            return EReupload::RequestStaging;
        case EReupload::RequestGpuUpload:
            m_Reupload = EReupload::NoAction;
            return EReupload::RequestGpuUpload;
        default:
            return EReupload::NoAction;
    }
}

// --------------------------------------------------------------------------------------------------------------------
EReupload MemoryUploadTracker::PeekStatus()
{
    return m_Reupload;
}

// --------------------------------------------------------------------------------------------------------------------
void MemoryUploadTracker::ForceUpload()
{
    if ( m_Reupload & EReupload::RequestStaging )
        return;

    m_Reupload = EReupload::RequestStaging;
}

} // namespace B33::Rendering
