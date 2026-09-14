#include "B33Rendering.h"

#include "Vulkan/Buffers/ImgBuffer.hpp"

namespace B33::Rendering
{

using namespace std;
using namespace B33;

// --------------------------------------------------------------------------------------------------------------------
ImgBuffer::ImgBuffer( __B33_ATTRIBUTE_MIGHT_BE_UNUSED const char *pszName )
  : m_pDeviceAdapter( nullptr )
  , m_Image( VK_NULL_HANDLE )
  , m_ImageView( VK_NULL_HANDLE )
  , m_Sampler( VK_NULL_HANDLE )
#if defined( _B33_DEBUG )
  , m_pszName( pszName )
#endif
{
}
// --------------------------------------------------------------------------------------------------------------------
ImgBuffer::ImgBuffer( SharedPtr<const AdapterWrapper>             da,
                      ::VkImage                                   image,
                      ::VkImageView                               imageView,
                      ::VkSampler                                 sampler,
                      __B33_ATTRIBUTE_MIGHT_BE_UNUSED const char *pszName )
  : m_pDeviceAdapter( da )
  , m_Image( image )
  , m_ImageView( imageView )
  , m_Sampler( sampler )
#if defined( _B33_DEBUG )
  , m_pszName( pszName )
#endif
{
}

// --------------------------------------------------------------------------------------------------------------------
ImgBuffer::~ImgBuffer() noexcept
{
    Free();
}

// --------------------------------------------------------------------------------------------------------------------
ImgBuffer::ImgBuffer( ImgBuffer &&other ) noexcept
  : m_pDeviceAdapter( std::move( other.m_pDeviceAdapter ) )
  , m_Image( other.m_Image )
  , m_ImageView( other.m_ImageView )
  , m_Sampler( other.m_Sampler )
{
    other.m_Image     = VK_NULL_HANDLE;
    other.m_ImageView = VK_NULL_HANDLE;
    other.m_Sampler   = VK_NULL_HANDLE;
}

// --------------------------------------------------------------------------------------------------------------------
ImgBuffer &ImgBuffer::operator=( ImgBuffer &&other ) noexcept
{
    this->~ImgBuffer();

    this->m_pDeviceAdapter = std::move( other.m_pDeviceAdapter );
    this->m_Image          = other.m_Image;
    this->m_ImageView      = other.m_ImageView;
    this->m_Sampler        = other.m_Sampler;

    return *this;
}

// --------------------------------------------------------------------------------------------------------------------
::VkImage ImgBuffer::GetImage() const
{
    B33_ASSERT( m_Image != VK_NULL_HANDLE );
    return m_Image;
}

// --------------------------------------------------------------------------------------------------------------------
::VkImageView ImgBuffer::GetImageView() const
{
    B33_ASSERT( m_ImageView != VK_NULL_HANDLE );
    return m_ImageView;
}

// --------------------------------------------------------------------------------------------------------------------
::VkSampler ImgBuffer::GetSampler() const
{
    B33_ASSERT( m_Sampler != VK_NULL_HANDLE );
    return m_Sampler;
}

// --------------------------------------------------------------------------------------------------------------------
const char *ImgBuffer::GetName() const
{
#if defined( _B33_DEBUG )
    return m_pszName;
#endif
    return nullptr;
}

// --------------------------------------------------------------------------------------------------------------------
::VkImage ImgBuffer::DetachImage()
{
    auto result = m_Image;
    m_Image     = VK_NULL_HANDLE;
    return result;
}

// --------------------------------------------------------------------------------------------------------------------
::VkImageView ImgBuffer::DetachImageView()
{
    auto result = m_ImageView;
    m_ImageView = VK_NULL_HANDLE;
    return result;
}

// --------------------------------------------------------------------------------------------------------------------
::VkSampler ImgBuffer::DetachSampler()
{
    auto result = m_Sampler;
    m_Sampler   = VK_NULL_HANDLE;
    return result;
}

// --------------------------------------------------------------------------------------------------------------------
void ImgBuffer::FreeImpl() noexcept
{
    if ( m_pDeviceAdapter && m_Sampler != VK_NULL_HANDLE )
    {
        B33_TRACE( L"Freeing sampler in img buffer" );
        ::vkDestroySampler( m_pDeviceAdapter->GetAdapterHandle(), m_Sampler, NULL );
    }
    if ( m_pDeviceAdapter && m_ImageView != VK_NULL_HANDLE )
    {
        B33_TRACE( L"Destroying imageview in img buffer" );
        ::vkDestroyImageView( m_pDeviceAdapter->GetAdapterHandle(), m_ImageView, NULL );
    }
    if ( m_pDeviceAdapter && m_Image != VK_NULL_HANDLE )
    {
        B33_TRACE( L"Freeing image in img buffer" );
        ::vkDestroyImage( m_pDeviceAdapter->GetAdapterHandle(), m_Image, NULL );
    }
    if ( m_pDeviceAdapter )
    {
        m_pDeviceAdapter = nullptr;
    }
}

} // namespace B33::Rendering
