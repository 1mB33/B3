#if !defined(B33_IMG_BUFFER_H)
#define B33_IMG_BUFFER_H

#include "Vulkan/WrapperAdapter.hpp"

namespace B33::Rendering
{

class ImgBuffer
{
  public:
    __B33_API ImgBuffer();

    __B33_API
    ImgBuffer( ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> da,
               ::VkImage                                                 image,
               ::VkImageView                                             imageView,
               ::VkSampler                                               sampler );

    __B33_API ~ImgBuffer();

  public:
    ImgBuffer( const ImgBuffer &other )                     = delete;
    ImgBuffer &operator=( const ImgBuffer &other ) noexcept = delete;

    ImgBuffer( ImgBuffer &&other ) noexcept;
    ImgBuffer &operator=( ImgBuffer &&other ) noexcept;

    // Getters // -----------------------------------------------------------------------------------------------------
  public:
    ::VkImage GetImage() const
    {
        B33_ASSERT( m_Image != VK_NULL_HANDLE );
        return m_Image;
    }

    ::VkImageView GetImageView() const
    {
        B33_ASSERT( m_ImageView != VK_NULL_HANDLE );
        return m_ImageView;
    }

    ::VkSampler GetSampler() const
    {
        B33_ASSERT( m_Sampler != VK_NULL_HANDLE );
        return m_Sampler;
    }

    ::VkImage DetachImage()
    {
        auto result = m_Image;
        m_Image     = VK_NULL_HANDLE;
        return result;
    }

    ::VkImageView DetachImageView()
    {
        auto result = m_ImageView;
        m_ImageView = VK_NULL_HANDLE;
        return result;
    }

    ::VkSampler DetachSampler()
    {
        auto result = m_Sampler;
        m_Sampler   = VK_NULL_HANDLE;
        return result;
    }

  protected:
    ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> m_pDeviceAdapter = nullptr;
    ::VkImage                                                 m_Image          = VK_NULL_HANDLE;
    ::VkImageView                                             m_ImageView      = VK_NULL_HANDLE;
    ::VkSampler                                               m_Sampler        = VK_NULL_HANDLE;
};

} // namespace B33::Rendering
#endif //! B33_IMG_BUFFER_H
