#ifndef B33_MEMORY_H
#define B33_MEMORY_H

#include "Vulkan/Buffers/GPUBuffer.hpp"
#include "Vulkan/Buffers/GPUStreamBuffer.hpp"
#include "Vulkan/Buffers/ImgBuffer.hpp"
#include "Vulkan/Memory/UploadDescriptor.hpp"
#include "Vulkan/WrapperAdapter.hpp"
#include "Vulkan/WrapperHardware.hpp"

namespace B33::Rendering
{

class Memory
{
  public:
    Memory() = default;

    Memory( ::std::shared_ptr<const ::B33::Rendering::HardwareWrapper> pHardware,
            ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper>  pAdapter );

    ~Memory();

  public:
    Memory( const Memory & ) noexcept            = default;
    Memory &operator=( const Memory & ) noexcept = default;

    Memory( Memory && ) noexcept            = default;
    Memory &operator=( Memory && ) noexcept = default;

    // Methods // -----------------------------------------------------------------------------------------------------
  public:
    __B33_API ::std::shared_ptr<::B33::Rendering::GPUBuffer> ReserveVertexBuffer( const size_t uSizeInBytes );

    __B33_API ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> ReserveStagingBuffer( const ::size_t uSizeInBytes );

    __B33_API ::std::shared_ptr<::B33::Rendering::GPUBuffer> ReserveGPUBuffer( const ::size_t uSizeInBytes );

    __B33_API ImgBuffer ReserveImage( const ::uint32_t          uWidth,
                                      const ::uint32_t          uHeigth,
                                      const ::VkFormat          format,
                                      const ::VkImageUsageFlags usage );

    __B33_API
    void ReserveImageView( ImgBuffer &image, const ::VkFormat format, const ::VkImageAspectFlags aspectMask );

    __B33_API
    void UploadToBufferRaw( const void                                                 *pUpload,
                            const ::size_t                                              uUploadSize,
                            const ::std::shared_ptr<::B33::Rendering::GPUBuffer> &gpuBuffer );

    __B33_API
    void UploadToStreamBufferRaw( const void                                                 *pUpload,
                                  const ::size_t                                              uUploadSize,
                                  const ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> &gpuStreamBuffer );

    __B33_API void UploadToStreamBufferDescSet( const void                               *pUpload,
                                                const ::size_t                            uUploadSize,
                                                const ::B33::Rendering::UploadDescriptor &onSet );

  private:
    ::uint32_t FindMemoryType( ::uint32_t typeFilter, ::VkMemoryPropertyFlags properties );

  private:
    ::std::shared_ptr<const ::B33::Rendering::HardwareWrapper> m_pHardware = nullptr;
    ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper>  m_pAdapter  = nullptr;
};

} // namespace B33::Rendering
#endif //! B33_MEMORY_H
