#include "B33Rendering.h"

#include "Vulkan/Buffers/GPUStreamBuffer.hpp"

namespace B33::Rendering
{

// Constructors // ----------------------------------------------------------------------------------------------------
GPUStreamBuffer::GPUStreamBuffer()
  : GPUBuffer()
  , m_pData( nullptr )
{
}

// --------------------------------------------------------------------------------------------------------------------
GPUStreamBuffer::GPUStreamBuffer( SharedPtr<const AdapterWrapper> da,
                                  VkDeviceMemory                  deviceMemory,
                                  VkBuffer                        buffer,
                                  void *,
                                  usize sizeInBytes )
  : GPUBuffer( da, deviceMemory, buffer, sizeInBytes )
{
}

// --------------------------------------------------------------------------------------------------------------------
GPUStreamBuffer::~GPUStreamBuffer() noexcept
{
    if ( m_pDeviceAdapter && m_pData != nullptr )
    {
        B33_TRACE( L"Unmapping memory in stream buffer" );
        ::vkUnmapMemory( m_pDeviceAdapter->GetAdapterHandle(), m_DeviceMemory );
        m_pData = nullptr;
    }

    GPUBuffer::~GPUBuffer();
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
    this->GPUBuffer::operator=( std::move( other ) );
    m_pData = other.m_pData;

    other.m_pData = nullptr;
    return *this;
}

// Public // ----------------------------------------------------------------------------------------------------------
void GPUStreamBuffer::Reset()
{
    ::vkUnmapMemory( this->m_pDeviceAdapter->GetAdapterHandle(), this->GetMemoryHandle() );
    m_pData = nullptr;
}

} // namespace B33::Rendering
