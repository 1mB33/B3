#if !defined( B33_IMG_BUFFER_HPP )
#    define B33_IMG_BUFFER_HPP

#    include "Vulkan/WrapperAdapter.hpp"

namespace B33::Rendering
{

class ImgBuffer
{
    template <typename T>
    using SharedPtr = ::std::shared_ptr<T>;

  public:
    __B33_API ImgBuffer();

    __B33_API
    ImgBuffer( SharedPtr<const AdapterWrapper> da, ::VkImage image, ::VkImageView imageView, ::VkSampler sampler );

    __B33_API ~ImgBuffer() noexcept;

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

  public:
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
    SharedPtr<const AdapterWrapper> m_pDeviceAdapter = nullptr;
    ::VkImage                       m_Image          = VK_NULL_HANDLE;
    ::VkImageView                   m_ImageView      = VK_NULL_HANDLE;
    ::VkSampler                     m_Sampler        = VK_NULL_HANDLE;
};

} // namespace B33::Rendering
#endif //! B33_IMG_BUFFER_HPP
