#include "B33Rendering.h"

#include "Vulkan/Buffers/GPUBuffer.hpp"

namespace B33::Rendering
{

// Constructors // ----------------------------------------------------------------------------------------------------
GPUBuffer::GPUBuffer( __B33_ATTRIBUTE_MIGHT_BE_UNUSED const char *pszName )
  : m_pDeviceAdapter( nullptr )
  , m_DeviceMemory( VK_NULL_HANDLE )
  , m_Buffer( VK_NULL_HANDLE )
  , m_uSizeInBytes( 0 )
#if defined( _B33_DEBUG )
  , m_pszName( pszName )
#endif
{
}

// --------------------------------------------------------------------------------------------------------------------
GPUBuffer::GPUBuffer( SharedPtr<const AdapterWrapper>             da,
                      ::VkDeviceMemory                            deviceMemory,
                      ::VkBuffer                                  buffer,
                      usize                                       sizeInBytes,
                      __B33_ATTRIBUTE_MIGHT_BE_UNUSED const char *pszName )
  : m_pDeviceAdapter( da )
  , m_DeviceMemory( deviceMemory )
  , m_Buffer( buffer )
  , m_uSizeInBytes( sizeInBytes )
#if defined( _B33_DEBUG )
  , m_pszName( pszName )
#endif
{
}

// --------------------------------------------------------------------------------------------------------------------
GPUBuffer::~GPUBuffer() noexcept
{
    FreeImpl();
}

// --------------------------------------------------------------------------------------------------------------------
GPUBuffer::GPUBuffer( GPUBuffer &&other ) noexcept
  : m_pDeviceAdapter( other.m_pDeviceAdapter )
  , m_DeviceMemory( other.m_DeviceMemory )
  , m_Buffer( other.m_Buffer )
  , m_uSizeInBytes( other.m_uSizeInBytes )
#if defined( _B33_DEBUG )
  , m_pszName( other.m_pszName )
#endif
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
#if defined( _B33_DEBUG )
    this->m_pszName = other.m_pszName;
#endif

    other.m_DeviceMemory = VK_NULL_HANDLE;
    other.m_Buffer       = VK_NULL_HANDLE;
#if defined( _B33_DEBUG )
    other.m_pszName = nullptr;
#endif

    return *this;
}

// --------------------------------------------------------------------------------------------------------------------
::VkDeviceMemory GPUBuffer::GetMemoryHandle() const
{
    return m_DeviceMemory;
}

// --------------------------------------------------------------------------------------------------------------------
::VkBuffer GPUBuffer::GetBufferHandle() const
{
    return m_Buffer;
}

// --------------------------------------------------------------------------------------------------------------------
usize GPUBuffer::GetSizeInBytes() const
{
    return m_uSizeInBytes;
}

// --------------------------------------------------------------------------------------------------------------------
const char *GPUBuffer::GetName() const
{
    return m_pszName;
}

// --------------------------------------------------------------------------------------------------------------------
void GPUBuffer::FreeImpl() noexcept
{
    B33_TRACE( L"Called free on %s", m_pszName );
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
} // namespace B33::Rendering
