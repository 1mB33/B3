#include "B33Rendering.h"

#include "Vulkan/Buffers/GPUStreamBuffer.hpp"

namespace B33::Rendering
{

// Constructors // ----------------------------------------------------------------------------------------------------
GPUStreamBuffer::GPUStreamBuffer( __B33_ATTRIBUTE_MIGHT_BE_UNUSED const char *pszName )
  : GPUBuffer()
  , m_pData( nullptr )
#if defined( _B33_DEBUG )
  , m_pszName( pszName )
#endif
{
}

// --------------------------------------------------------------------------------------------------------------------
GPUStreamBuffer::GPUStreamBuffer( SharedPtr<const AdapterWrapper> da,
                                  VkDeviceMemory                  deviceMemory,
                                  VkBuffer                        buffer,
                                  void *,
                                  usize                                       sizeInBytes,
                                  __B33_ATTRIBUTE_MIGHT_BE_UNUSED const char *pszName )
  : GPUBuffer( da, deviceMemory, buffer, sizeInBytes )
#if defined( _B33_DEBUG )
  , m_pszName( pszName )
#endif
{
}

// --------------------------------------------------------------------------------------------------------------------
GPUStreamBuffer::~GPUStreamBuffer() noexcept
{
    FreeImpl();
}

// --------------------------------------------------------------------------------------------------------------------
GPUStreamBuffer::GPUStreamBuffer( GPUStreamBuffer &&other ) noexcept
  : GPUBuffer( std::move( other ) )
  , m_pData( other.m_pData )
{
}

// --------------------------------------------------------------------------------------------------------------------
GPUStreamBuffer &GPUStreamBuffer::operator=( GPUStreamBuffer &&other ) noexcept
{
    GPUBuffer::operator=( std::move( other ) );
    m_pData = other.m_pData;

    other.m_pData = nullptr;
    return *this;
}

// Public // ----------------------------------------------------------------------------------------------------------
void *GPUStreamBuffer::GetDataPointer() const
{
    return m_pData;
}

// --------------------------------------------------------------------------------------------------------------------
void **GPUStreamBuffer::GetPtrToDataPointer()
{
    return &m_pData;
}

// --------------------------------------------------------------------------------------------------------------------
const char *GPUStreamBuffer::GetName() const
{
    return m_pszName;
}

// --------------------------------------------------------------------------------------------------------------------
void GPUStreamBuffer::Reset()
{
    ::vkUnmapMemory( this->m_pDeviceAdapter->GetAdapterHandle(), this->GetMemoryHandle() );
    m_pData = nullptr;
}

// --------------------------------------------------------------------------------------------------------------------
void GPUStreamBuffer::FreeImpl() noexcept
{
    B33_TRACE( L"Called free on stream buffer %s", m_pszName );
    if ( m_pDeviceAdapter && m_pData != nullptr )
    {
        B33_TRACE( L"Unmapping memory in stream buffer" );
        ::vkUnmapMemory( m_pDeviceAdapter->GetAdapterHandle(), m_DeviceMemory );
        m_pData = nullptr;
    }

    GPUBuffer::FreeImpl();
}

} // namespace B33::Rendering
