#ifndef B33_GPU_STREAM_BUFFER_H
#define B33_GPU_STREAM_BUFFER_H

#include "Vulkan/Buffers/GPUBuffer.hpp"

namespace B33::Rendering
{

class GPUStreamBuffer : public ::B33::Rendering::GPUBuffer
{
  public:
    __B33_API GPUStreamBuffer();
    __B33_API GPUStreamBuffer( ::std::shared_ptr<const AdapterWrapper> da,
                               ::VkDeviceMemory                        deviceMemory,
                               ::VkBuffer                              buffer,
                               void *,
                               ::size_t sizeInBytes );

    __B33_API ~GPUStreamBuffer();

  public:
    GPUStreamBuffer( const GPUStreamBuffer &other )                     = delete;
    GPUStreamBuffer &operator=( const GPUStreamBuffer &other ) noexcept = delete;

    __B33_API                  GPUStreamBuffer( GPUStreamBuffer &&other ) noexcept;
    __B33_API GPUStreamBuffer &operator=( GPUStreamBuffer &&other ) noexcept;

    // Getters // -----------------------------------------------------------------------------------------------------
  public:
    void *GetDataPointer() const
    {
        return m_pData;
    }

    void **GetPtrToDataPointer()
    {
        return &m_pData;
    }

    // Methods // -----------------------------------------------------------------------------------------------------
  public:
    __B33_API void Reset();

  private:
    void *m_pData = nullptr;
};

} // namespace B33::Rendering
#endif // !B33_GPU_STREAM_BUFFER_H
