#if !defined( B33_GPU_STREAM_BUFFER_HPP )
#    define B33_GPU_STREAM_BUFFER_HPP

#    include "Vulkan/Buffers/GPUBuffer.hpp"

namespace B33::Rendering
{

class GPUStreamBuffer : public GPUBuffer
{
    template <typename T>
    using SharedPtr = ::std::shared_ptr<T>;

  public:
    __B33_API GPUStreamBuffer();
    __B33_API GPUStreamBuffer( SharedPtr<const AdapterWrapper> da,
                               ::VkDeviceMemory                deviceMemory,
                               ::VkBuffer                      buffer,
                               void *,
                               usize sizeInBytes );

    __B33_API ~GPUStreamBuffer() noexcept;

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
#endif // !B33_GPU_STREAM_BUFFER_HPP
