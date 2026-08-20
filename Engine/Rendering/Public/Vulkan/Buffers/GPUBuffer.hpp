#ifndef B33_GPU_BUFFER_H
#define B33_GPU_BUFFER_H

#include "Vulkan/WrapperAdapter.hpp"

namespace B33::Rendering
{

class GPUBuffer
{
  public:
    __B33_API GPUBuffer();
    __B33_API GPUBuffer( ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> da,
                         ::VkDeviceMemory                                          deviceMemory,
                         ::VkBuffer                                                buffer,
                         ::size_t                                                  sizeInBytes );

    __B33_API ~GPUBuffer();

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

    ::size_t GetSizeInBytes() const
    {
        return m_uSizeInBytes;
    }

  protected:
    ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> m_pDeviceAdapter = nullptr;
    ::VkDeviceMemory                                          m_DeviceMemory   = VK_NULL_HANDLE;
    ::VkBuffer                                                m_Buffer         = VK_NULL_HANDLE;
    ::size_t                                                  m_uSizeInBytes   = 0;
};

} // namespace B33::Rendering
#endif //! B33_GPU_BUFFER_H
