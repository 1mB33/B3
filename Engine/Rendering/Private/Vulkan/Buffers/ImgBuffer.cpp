#include "B33Rendering.hpp"

#include "Vulkan/Buffers/ImgBuffer.hpp"

namespace B33::Rendering
{

using namespace std;
using namespace B33;

ImgBuffer::ImgBuffer()
  : m_pDeviceAdapter( nullptr )
  , m_Image( VK_NULL_HANDLE )
  , m_ImageView( VK_NULL_HANDLE )
  , m_Sampler( VK_NULL_HANDLE )
{
}

ImgBuffer::ImgBuffer( ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> da,
                      ::VkImage                                                 image,
                      ::VkImageView                                             imageView,
                      ::VkSampler                                               sampler )
  : m_pDeviceAdapter( da )
  , m_Image( image )
  , m_ImageView( imageView )
  , m_Sampler( sampler )
{
}

ImgBuffer::~ImgBuffer()
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

ImgBuffer &ImgBuffer::operator=( ImgBuffer &&other ) noexcept
{
    this->~ImgBuffer();

    this->m_pDeviceAdapter = std::move( other.m_pDeviceAdapter );
    this->m_Image          = other.m_Image;
    this->m_ImageView      = other.m_ImageView;
    this->m_Sampler        = other.m_Sampler;

    return *this;
}

} // namespace B33::Rendering
