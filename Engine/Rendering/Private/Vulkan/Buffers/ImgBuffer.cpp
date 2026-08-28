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
{
}

ImgBuffer::ImgBuffer( ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> da,
                      ::VkImage                                                 image,
                      ::VkImageView                                             imageView )
  : m_pDeviceAdapter( da )
  , m_Image( image )
  , m_ImageView( imageView )
{
}

ImgBuffer::~ImgBuffer()
{
    if ( m_pDeviceAdapter && m_ImageView != VK_NULL_HANDLE )
    {
        B33_TRACE( L"Destroying buffer in GPU buffer" );
        ::vkDestroyImageView( m_pDeviceAdapter->GetAdapterHandle(), m_ImageView, NULL );
    }
    if ( m_pDeviceAdapter && m_Image != VK_NULL_HANDLE )
    {
        B33_TRACE( L"Freeing memory in GPU buffer" );
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
{
    other.m_Image     = VK_NULL_HANDLE;
    other.m_ImageView = VK_NULL_HANDLE;
}

ImgBuffer &ImgBuffer::operator=( ImgBuffer &&other ) noexcept
{
    this->~ImgBuffer();

    this->m_pDeviceAdapter = std::move( other.m_pDeviceAdapter );
    this->m_Image          = other.m_Image;
    this->m_ImageView      = other.m_ImageView;

    return *this;
}

} // namespace B33::Rendering
