#if !defined( B33_GPU_BUFFER_HPP )
#    define B33_GPU_BUFFER_HPP

#    include "Vulkan/WrapperAdapter.hpp"

namespace B33::Rendering
{

class GPUBuffer
{
    template <typename T>
    using SharedPtr = ::std::shared_ptr<T>;

  public:
    __B33_API GPUBuffer();
    __B33_API GPUBuffer( SharedPtr<const AdapterWrapper> da,
                         ::VkDeviceMemory                deviceMemory,
                         ::VkBuffer                      buffer,
                         usize                           sizeInBytes );

    __B33_API ~GPUBuffer() noexcept;

  public:
    GPUBuffer( const GPUBuffer &other )                     = delete;
    GPUBuffer &operator=( const GPUBuffer &other ) noexcept = delete;

    GPUBuffer( GPUBuffer &&other ) noexcept;
    GPUBuffer &operator=( GPUBuffer &&other ) noexcept;

    // Getters // -----------------------------------------------------------------------------------------------------
  public:
    ::VkDeviceMemory GetMemoryHandle() const
    {
        return m_DeviceMemory;
    }

    ::VkBuffer GetBufferHandle() const
    {
        return m_Buffer;
    }

    usize GetSizeInBytes() const
    {
        return m_uSizeInBytes;
    }

  protected:
    SharedPtr<const AdapterWrapper> m_pDeviceAdapter = nullptr;
    ::VkDeviceMemory                m_DeviceMemory   = VK_NULL_HANDLE;
    ::VkBuffer                      m_Buffer         = VK_NULL_HANDLE;
    usize                           m_uSizeInBytes   = 0;
};

} // namespace B33::Rendering
#endif //! B33_GPU_BUFFER_HPP
