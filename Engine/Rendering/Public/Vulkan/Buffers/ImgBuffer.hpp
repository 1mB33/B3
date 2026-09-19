#if !defined( B33_IMG_BUFFER_HPP )
#    define B33_IMG_BUFFER_HPP

#    include "Vulkan/Buffers/IMemoryBuffer.hpp"
#    include "Vulkan/WrapperAdapter.hpp"

namespace B33::Rendering
{

class ImgBuffer : public IMemoryBuffer<ImgBuffer>
{
    template <typename T>
    using SharedPtr = ::std::shared_ptr<T>;

  public:
    __B33_API ImgBuffer( const char *pszName = nullptr );

    __B33_API
    ImgBuffer( SharedPtr<const AdapterWrapper> da,
               ::VkImage                       image,
               ::VkImageView                   imageView,
               ::VkSampler                     sampler,
               const char                     *pszName = nullptr );

    __B33_API ~ImgBuffer() noexcept;

  public:
    ImgBuffer( const ImgBuffer &other )                     = delete;
    ImgBuffer &operator=( const ImgBuffer &other ) noexcept = delete;

    __B33_API            ImgBuffer( ImgBuffer &&other ) noexcept;
    __B33_API ImgBuffer &operator=( ImgBuffer &&other ) noexcept;

    // Getters // -----------------------------------------------------------------------------------------------------
  public:
    __B33_API ::VkImage GetImage() const;

    __B33_API ::VkImageView GetImageView() const;

    __B33_API ::VkSampler GetSampler() const;

    __B33_API const char *GetName() const;

    // Setters // -----------------------------------------------------------------------------------------------------
  public:
    inline void SetName( __B33_ATTRIBUTE_MIGHT_BE_UNUSED const char *pszName )
    {
#    if defined( _B33_DEBUG )
        m_pszName = pszName;
#    endif
    }

  public:
    __B33_API ::VkImage DetachImage();

    __B33_API ::VkImageView DetachImageView();

    __B33_API ::VkSampler DetachSampler();

    __B33_API void FreeImpl() noexcept;

  protected:
    SharedPtr<const AdapterWrapper> m_pDeviceAdapter = nullptr;
    ::VkImage                       m_Image          = VK_NULL_HANDLE;
    ::VkImageView                   m_ImageView      = VK_NULL_HANDLE;
    ::VkSampler                     m_Sampler        = VK_NULL_HANDLE;

#    if defined( _B33_DEBUG )
    const char *m_pszName = nullptr;
#    endif
};

} // namespace B33::Rendering
#endif //! B33_IMG_BUFFER_HPP
