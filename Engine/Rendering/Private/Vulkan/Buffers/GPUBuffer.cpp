#include "B33Rendering.h"

#include "Vulkan/Buffers/GPUBuffer.hpp"

namespace B33::Rendering
{

// Constructors // ----------------------------------------------------------------------------------------------------
GPUBuffer::GPUBuffer()
  : m_pDeviceAdapter( nullptr )
  , m_DeviceMemory( VK_NULL_HANDLE )
  , m_Buffer( VK_NULL_HANDLE )
  , m_uSizeInBytes( 0 )
{
}

// --------------------------------------------------------------------------------------------------------------------
GPUBuffer::GPUBuffer( SharedPtr<const AdapterWrapper> da,
                      ::VkDeviceMemory                deviceMemory,
                      ::VkBuffer                      buffer,
                      usize                           sizeInBytes )
  : m_pDeviceAdapter( da )
  , m_DeviceMemory( deviceMemory )
  , m_Buffer( buffer )
  , m_uSizeInBytes( sizeInBytes )
{
}

// --------------------------------------------------------------------------------------------------------------------
GPUBuffer::~GPUBuffer() noexcept
{
    if ( m_pDeviceAdapter && m_Buffer != VK_NULL_HANDLE )
    {
        B33_TRACE( L"Destroying buffer in GPU buffer" );
        ::vkDestroyBuffer( m_pDeviceAdapter->GetAdapterHandle(), m_Buffer, NULL );
    }
    if ( m_pDeviceAdapter && m_DeviceMemory != VK_NULL_HANDLE )
    {
        B33_TRACE( L"Freeing memory in GPU buffer" );
        ::vkFreeMemory( m_pDeviceAdapter->GetAdapterHandle(), m_DeviceMemory, NULL );
    }
    if ( m_pDeviceAdapter )
    {
        m_pDeviceAdapter = nullptr;
    }
}

// --------------------------------------------------------------------------------------------------------------------
GPUBuffer::GPUBuffer( GPUBuffer &&other ) noexcept
  : m_pDeviceAdapter( other.m_pDeviceAdapter )
  , m_DeviceMemory( other.m_DeviceMemory )
  , m_Buffer( other.m_Buffer )
  , m_uSizeInBytes( other.m_uSizeInBytes )
{
    other.m_DeviceMemory = VK_NULL_HANDLE;
    other.m_Buffer       = VK_NULL_HANDLE;
}

// --------------------------------------------------------------------------------------------------------------------
GPUBuffer &GPUBuffer::operator=( GPUBuffer &&other ) noexcept
{
    this->m_pDeviceAdapter = other.m_pDeviceAdapter;
    this->m_DeviceMemory   = other.m_DeviceMemory;
    this->m_Buffer         = other.m_Buffer;
    this->m_uSizeInBytes   = other.m_uSizeInBytes;

    other.m_DeviceMemory = VK_NULL_HANDLE;
    other.m_Buffer       = VK_NULL_HANDLE;

    return *this;
}

} // namespace B33::Rendering
