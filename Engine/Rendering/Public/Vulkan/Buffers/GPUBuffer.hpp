#if !defined( B33_GPU_BUFFER_HPP )
#    define B33_GPU_BUFFER_HPP

#    include "IMemoryBuffer.hpp"
#    include "Vulkan/WrapperAdapter.hpp"

namespace B33::Rendering
{

class GPUBuffer : public IMemoryBuffer<GPUBuffer>
{
    template <typename T>
    using SharedPtr = ::std::shared_ptr<T>;

  public:
    __B33_API GPUBuffer( const char *pszName = nullptr );
    __B33_API GPUBuffer( SharedPtr<const AdapterWrapper> da,
                         ::VkDeviceMemory                deviceMemory,
                         ::VkBuffer                      buffer,
                         usize                           sizeInBytes,
                         const char                     *pszName = nullptr );

    __B33_API ~GPUBuffer() noexcept;

  public:
    GPUBuffer( const GPUBuffer &other )                     = delete;
    GPUBuffer &operator=( const GPUBuffer &other ) noexcept = delete;

    __B33_API            GPUBuffer( GPUBuffer &&other ) noexcept;
    __B33_API GPUBuffer &operator=( GPUBuffer &&other ) noexcept;

    // Getters // -----------------------------------------------------------------------------------------------------
  public:
    __B33_API ::VkDeviceMemory GetMemoryHandle() const;

    __B33_API ::VkBuffer GetBufferHandle() const;

    __B33_API usize GetSizeInBytes() const;

    __B33_API const char *GetName() const;

    // Setters // -----------------------------------------------------------------------------------------------------
  public:
    inline void SetName( const char *pszName )
    {
#    if defined( _B33_DEBUG )
        m_pszName = pszName;
#    endif
    }

  public:
    __B33_API void FreeImpl() noexcept;

  protected:
    SharedPtr<const AdapterWrapper> m_pDeviceAdapter = nullptr;
    ::VkDeviceMemory                m_DeviceMemory   = VK_NULL_HANDLE;
    ::VkBuffer                      m_Buffer         = VK_NULL_HANDLE;
    usize                           m_uSizeInBytes   = 0;

  private:
#    if defined( _B33_DEBUG )
    const char *m_pszName = nullptr;
#    endif
};

} // namespace B33::Rendering
#endif //! B33_GPU_BUFFER_HPP
