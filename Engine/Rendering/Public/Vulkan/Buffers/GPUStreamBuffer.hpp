#if !defined( B33_GPU_STREAM_BUFFER_HPP )
#    define B33_GPU_STREAM_BUFFER_HPP

#    include "Vulkan/Buffers/GPUBuffer.hpp"

namespace B33::Rendering
{

class GPUStreamBuffer
  : private GPUBuffer
  , public IMemoryBuffer<GPUStreamBuffer>
{
    template <typename T>
    using SharedPtr = ::std::shared_ptr<T>;

  public:
    __B33_API GPUStreamBuffer( const char *pszName = nullptr );
    __B33_API GPUStreamBuffer( SharedPtr<const AdapterWrapper> da,
                               ::VkDeviceMemory                deviceMemory,
                               ::VkBuffer                      buffer,
                               void *,
                               usize       sizeInBytes,
                               const char *pszName = nullptr );

    __B33_API ~GPUStreamBuffer() noexcept;

  public:
    GPUStreamBuffer( const GPUStreamBuffer &other )                     = delete;
    GPUStreamBuffer &operator=( const GPUStreamBuffer &other ) noexcept = delete;

    __B33_API                  GPUStreamBuffer( GPUStreamBuffer &&other ) noexcept;
    __B33_API GPUStreamBuffer &operator=( GPUStreamBuffer &&other ) noexcept;

    // Getters // -----------------------------------------------------------------------------------------------------
  public:
    using GPUBuffer::GetBufferHandle;
    using GPUBuffer::GetMemoryHandle;
    using GPUBuffer::GetSizeInBytes;

    __B33_API void *GetDataPointer() const;

    __B33_API void **GetPtrToDataPointer();

    __B33_API const char *GetName() const;

    // Setters // -----------------------------------------------------------------------------------------------------
  public:
    inline void SetName( __B33_ATTRIBUTE_MIGHT_BE_UNUSED const char *pszName )
    {
#    if defined( _B33_DEBUG )
        m_pszName = pszName;
#    endif
    }

    // Methods // -----------------------------------------------------------------------------------------------------
  public:
    using IMemoryBuffer<GPUStreamBuffer>::Free;

    __B33_API void Reset();

    __B33_API void FreeImpl() noexcept;


  private:
    void *m_pData = nullptr;

#    if defined( _B33_DEBUG )
    const char *m_pszName = nullptr;
#    endif
};

} // namespace B33::Rendering
#endif // !B33_GPU_STREAM_BUFFER_HPP
