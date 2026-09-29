#if !defined( B33_MEMORY_HPP )
#    define B33_MEMORY_HPP

#    include "Vulkan/Buffers/GPUBuffer.hpp"
#    include "Vulkan/Buffers/GPUStreamBuffer.hpp"
#    include "Vulkan/Buffers/ImgBuffer.hpp"
#    include "Vulkan/Memory/UploadDescriptor.hpp"
#    include "Vulkan/WrapperAdapter.hpp"
#    include "Vulkan/WrapperHardware.hpp"

namespace B33::Rendering
{

class Memory
{
    template <typename T>
    using SharedPtr = ::std::shared_ptr<T>;
    template <class T>
    using BufferList = ::std::vector<::std::weak_ptr<T>>;

  public:
    Memory();

    Memory( SharedPtr<const HardwareWrapper> pHardware, SharedPtr<const AdapterWrapper> pAdapter );

    ~Memory() noexcept;

  public:
    Memory( const Memory & ) noexcept;
    Memory &operator=( const Memory & ) noexcept;

    Memory( Memory && ) noexcept;
    Memory &operator=( Memory && ) noexcept;

    // Methods // -----------------------------------------------------------------------------------------------------
  public:
    __B33_API void Reset() noexcept;

    __B33_API SharedPtr<GPUBuffer> ReserveVertexBuffer( const usize uSizeInBytes );

    __B33_API SharedPtr<GPUBuffer> ReserveStagingBuffer( const usize uSizeInBytes );

    __B33_API SharedPtr<GPUStreamBuffer> ReserveStreamStagingBuffer( const usize uSizeInBytes );

    __B33_API SharedPtr<GPUBuffer> ReserveGPUBuffer( const usize uSizeInBytes );

    __B33_API SharedPtr<ImgBuffer>
    ReserveImage( const u32 uWidth, const u32 uHeigth, const ::VkFormat format, const ::VkImageUsageFlags usage );

    __B33_API
    void ReserveImageView( ImgBuffer &image, const ::VkFormat format, const ::VkImageAspectFlags aspectMask );

    __B33_API
    void ReserveSampler( ImgBuffer &image, VkSamplerCreateInfo &createInfo );

    __B33_API
    void UploadToBufferRaw( const void *pUpload, const usize uUploadSize, const SharedPtr<GPUBuffer> &gpuBuffer );

    __B33_API
    void UploadToStreamBufferRaw( const void                       *pUpload,
                                  const usize                       uUploadSize,
                                  const SharedPtr<GPUStreamBuffer> &gpuStreamBuffer );

    __B33_API void UploadToStreamBufferDescSet( const void                              *pUpload,
                                                const usize                              uUploadSize,
                                                const UploadDescriptor<GPUStreamBuffer> &onSet );

  private:
    u32 FindMemoryType( u32 typeFilter, ::VkMemoryPropertyFlags properties );

  private:
    SharedPtr<const HardwareWrapper> m_pHardware = nullptr;
    SharedPtr<const AdapterWrapper>  m_pAdapter  = nullptr;

    BufferList<GPUBuffer>       m_GPUBuffers       = {};
    BufferList<GPUStreamBuffer> m_GPUStreamBuffers = {};
    BufferList<ImgBuffer>       m_ImageBuffers     = {};
};

} // namespace B33::Rendering
#endif //! B33_MEMORY_HPP
