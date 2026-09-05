#include "B33Rendering.h"

#include "Vulkan/Memory/UploadDescriptor.hpp"

namespace B33::Rendering
{

using namespace std;
using namespace B33::Core;
using namespace B33::Core::Debug;

// Constructors // ----------------------------------------------------------------------------------------------------
UploadDescriptor::UploadDescriptor( const UploadDescriptor &other ) noexcept
  : BufferInfo( other.BufferInfo )
  , Write( other.Write )
  , Type( other.Type )
{
    Write.pBufferInfo = &BufferInfo;
    Buffer            = other.Buffer;
}

// --------------------------------------------------------------------------------------------------------------------
UploadDescriptor &UploadDescriptor::operator=( const UploadDescriptor &other ) noexcept
{
    BufferInfo        = other.BufferInfo;
    Write             = other.Write;
    Type              = other.Type;
    Write.pBufferInfo = &BufferInfo;
    Buffer            = other.Buffer;

    return *this;
}

// --------------------------------------------------------------------------------------------------------------------
UploadDescriptor::UploadDescriptor( UploadDescriptor &&other ) noexcept
  : BufferInfo( std::move( other.BufferInfo ) )
  , Write( std::move( other.Write ) )
  , Type( other.Type )
{
    Write.pBufferInfo = &BufferInfo;
    Buffer            = std::move( other.Buffer );
}

// --------------------------------------------------------------------------------------------------------------------
UploadDescriptor &UploadDescriptor::operator=( UploadDescriptor &&other ) noexcept
{
    BufferInfo        = std::move( other.BufferInfo );
    Write             = std::move( other.Write );
    Type              = std::move( other.Type );
    Write.pBufferInfo = &BufferInfo;
    Buffer            = other.Buffer;

    return *this;
}

} // namespace B33::Rendering
