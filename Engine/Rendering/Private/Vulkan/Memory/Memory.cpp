#include "B33Rendering.h"

#include "Vulkan/Buffers/ImgBuffer.hpp"
#include "Vulkan/ErrorHandling.hpp"
#include "Vulkan/Memory/Memory.hpp"
#include "vulkan/vulkan_core.h"

namespace B33::Rendering
{

using namespace std;
using namespace B33::Core;
using namespace B33::Core::Debug;

// Constructors // ----------------------------------------------------------------------------------------------------
Memory::Memory( SharedPtr<const HardwareWrapper> pHardware, SharedPtr<const AdapterWrapper> pAdapter )
  : m_pHardware( pHardware )
  , m_pAdapter( pAdapter )
{
    B33_INFO( L"Initializing memory" );
}

// --------------------------------------------------------------------------------------------------------------------
Memory::~Memory() noexcept
{
    B33_INFO( L"Destroying memory" );
}

// --------------------------------------------------------------------------------------------------------------------
shared_ptr<GPUStreamBuffer> Memory::ReserveStagingBuffer( const usize uSizeInBytes )
{
    B33_LOG( Info, L"Reserving staging buffer of %llu bytes", uSizeInBytes );

    const VkDevice       da = m_pAdapter->GetAdapterHandle();
    VkMemoryRequirements memRequirements;
    VkBuffer             voxelBuffer;
    VkDeviceMemory       voxelBufferMemory;

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType              = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size               = uSizeInBytes;
    bufferInfo.usage              = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferInfo.sharingMode        = VK_SHARING_MODE_EXCLUSIVE;

    THROW_IF_FAILED( vkCreateBuffer( da, &bufferInfo, NULL, &voxelBuffer ) );

    vkGetBufferMemoryRequirements( da, voxelBuffer, &memRequirements );

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType                = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize       = memRequirements.size;
    allocInfo.memoryTypeIndex =
        FindMemoryType( memRequirements.memoryTypeBits,
                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT );

    THROW_IF_FAILED( vkAllocateMemory( da, &allocInfo, NULL, &voxelBufferMemory ) );
    THROW_IF_FAILED( vkBindBufferMemory( da, voxelBuffer, voxelBufferMemory, 0 ) );

    return make_shared<GPUStreamBuffer>( m_pAdapter, voxelBufferMemory, voxelBuffer, nullptr, uSizeInBytes );
}

// --------------------------------------------------------------------------------------------------------------------
shared_ptr<GPUBuffer> Memory::ReserveVertexBuffer( const usize uSizeInBytes )
{
    B33_LOG( Info, L"Reserving gpu buffer of %llu bytes", uSizeInBytes );

    const VkDevice       da = m_pAdapter->GetAdapterHandle();
    VkMemoryRequirements memRequirements;
    VkBuffer             buffer;
    VkDeviceMemory       deviceMem;

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType              = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size               = uSizeInBytes;
    bufferInfo.usage =
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    THROW_IF_FAILED( vkCreateBuffer( da, &bufferInfo, NULL, &buffer ) );

    vkGetBufferMemoryRequirements( da, buffer, &memRequirements );

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType                = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize       = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType( memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );

    THROW_IF_FAILED( vkAllocateMemory( da, &allocInfo, NULL, &deviceMem ) );
    THROW_IF_FAILED( vkBindBufferMemory( da, buffer, deviceMem, 0 ) );

    return make_shared<GPUBuffer>( m_pAdapter, deviceMem, buffer, uSizeInBytes );
}

// --------------------------------------------------------------------------------------------------------------------
shared_ptr<GPUBuffer> Memory::ReserveGPUBuffer( const usize uSizeInBytes )
{
    B33_LOG( Info, L"Reserving gpu buffer of %llu bytes", uSizeInBytes );

    const VkDevice       da = m_pAdapter->GetAdapterHandle();
    VkMemoryRequirements memRequirements;
    VkBuffer             buffer;
    VkDeviceMemory       deviceMem;

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType              = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size               = uSizeInBytes;
    bufferInfo.usage              = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    bufferInfo.sharingMode        = VK_SHARING_MODE_EXCLUSIVE;

    THROW_IF_FAILED( vkCreateBuffer( da, &bufferInfo, NULL, &buffer ) );

    vkGetBufferMemoryRequirements( da, buffer, &memRequirements );

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType                = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize       = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType( memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );

    THROW_IF_FAILED( vkAllocateMemory( da, &allocInfo, NULL, &deviceMem ) );
    THROW_IF_FAILED( vkBindBufferMemory( da, buffer, deviceMem, 0 ) );

    return make_shared<GPUBuffer>( m_pAdapter, deviceMem, buffer, uSizeInBytes );
}

// --------------------------------------------------------------------------------------------------------------------
ImgBuffer
Memory::ReserveImage( const u32 uWidth, const u32 uHeigth, const VkFormat format, const VkImageUsageFlags usage )
{
    const VkDevice    da   = m_pAdapter->GetAdapterHandle();
    VkImageCreateInfo info = {};
    VkImage           result;

    info.sType       = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    info.imageType   = VK_IMAGE_TYPE_2D;
    info.format      = format;
    info.extent      = VkExtent3D { uWidth, uHeigth, 1 };
    info.usage       = usage;
    info.mipLevels   = 1;
    info.arrayLayers = 1;
    info.samples     = VK_SAMPLE_COUNT_1_BIT;

    THROW_IF_FAILED( vkCreateImage( da, &info, NULL, &result ) );

    VkMemoryRequirements memReq;
    vkGetImageMemoryRequirements( da, result, &memReq );

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType                = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize       = memReq.size;
    allocInfo.memoryTypeIndex      = FindMemoryType( memReq.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );

    VkDeviceMemory memory;
    THROW_IF_FAILED( vkAllocateMemory( da, &allocInfo, NULL, &memory ) );
    THROW_IF_FAILED( vkBindImageMemory( da, result, memory, 0 ) );

    return ImgBuffer( m_pAdapter, result, VK_NULL_HANDLE, VK_NULL_HANDLE );
}

// --------------------------------------------------------------------------------------------------------------------
void Memory::UploadToBufferRaw( const void *pUpload, const usize uUploadSize, const SharedPtr<GPUBuffer> &gpuBuffer )
{
    const VkDevice da     = m_pAdapter->GetAdapterHandle();
    GPUBuffer     *buffer = gpuBuffer.get();
    void          *pData  = nullptr;

    THROW_IF_FAILED( vkMapMemory( da, buffer->GetMemoryHandle(), 0, buffer->GetSizeInBytes(), 0, &pData ) );
    memcpy( pData, pUpload, uUploadSize );
    vkUnmapMemory( da, buffer->GetMemoryHandle() );
}

// --------------------------------------------------------------------------------------------------------------------
void Memory::ReserveImageView( ImgBuffer &image, const VkFormat format, const VkImageAspectFlags aspectMask )
{
    const VkDevice        da   = m_pAdapter->GetAdapterHandle();
    VkImageViewCreateInfo info = {};
    VkImageView           result;

    info.sType                       = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.image                       = image.GetImage();
    info.viewType                    = VK_IMAGE_VIEW_TYPE_2D;
    info.format                      = format;
    info.subresourceRange.aspectMask = aspectMask;
    info.subresourceRange.levelCount = 1;
    info.subresourceRange.layerCount = 1;

    THROW_IF_FAILED( vkCreateImageView( da, &info, NULL, &result ) );

    image = ImgBuffer( m_pAdapter, image.DetachImage(), result, image.DetachSampler() );
}

void Memory::ReserveSampler( ImgBuffer &image, VkSamplerCreateInfo &createInfo )
{
    const VkDevice da = m_pAdapter->GetAdapterHandle();
    VkSampler      result;

    THROW_IF_FAILED( vkCreateSampler( da, &createInfo, NULL, &result ) );

    image = ImgBuffer( m_pAdapter, image.DetachImage(), image.DetachImageView(), result );
}

// --------------------------------------------------------------------------------------------------------------------
void Memory::UploadToStreamBufferRaw( const void                       *pUpload,
                                      const usize                       uUploadSize,
                                      const SharedPtr<GPUStreamBuffer> &gpuStreamBuffer )
{
    const VkDevice   da     = m_pAdapter->GetAdapterHandle();
    GPUStreamBuffer *buffer = gpuStreamBuffer.get();

    if ( buffer->GetDataPointer() == nullptr )
    {
        THROW_IF_FAILED( vkMapMemory( da,
                                      buffer->GetMemoryHandle(),
                                      0,
                                      buffer->GetSizeInBytes(),
                                      0,
                                      buffer->GetPtrToDataPointer() ) );
    }
    memcpy( buffer->GetDataPointer(), pUpload, uUploadSize );
}

// --------------------------------------------------------------------------------------------------------------------
void Memory::UploadToStreamBufferDescSet( const void *pUpload, const usize uUploadSize, const UploadDescriptor &onSet )
{
    B33_ASSERT( onSet.Buffer->GetMemoryHandle() != VK_NULL_HANDLE );
    B33_ASSERT( onSet.Buffer->GetBufferHandle() != VK_NULL_HANDLE );
    B33_ASSERT( onSet.Buffer->GetSizeInBytes() >= uUploadSize );

    if ( onSet.Type != EUploadType::StreamBuffer )
    {
        Logger::Get().Log( Error, L"UploadOnStreamBuffer, type of buffer is invalid type" );
        return;
    }

    const VkDevice   da     = m_pAdapter->GetAdapterHandle();
    GPUStreamBuffer *buffer = reinterpret_cast<GPUStreamBuffer *>( onSet.Buffer.get() );

    if ( buffer->GetDataPointer() == nullptr )
    {
        THROW_IF_FAILED( vkMapMemory( da,
                                      buffer->GetMemoryHandle(),
                                      0,
                                      buffer->GetSizeInBytes(),
                                      0,
                                      buffer->GetPtrToDataPointer() ) );
    }
    memcpy( buffer->GetDataPointer(), pUpload, uUploadSize );
    vkUpdateDescriptorSets( da, 1, &onSet.Write, 0, NULL );
}

// ---------------------------------------------------------------------------------------------------------------------
u32 Memory::FindMemoryType( u32 typeFilter, VkMemoryPropertyFlags properties )
{
    VkPhysicalDeviceMemoryProperties memProperties;

    vkGetPhysicalDeviceMemoryProperties( m_pHardware->GetPhysicalDevice(), &memProperties );

    for ( u32 i = 0; i < memProperties.memoryTypeCount; ++i )
    {
        bool bTypeMatch       = ( typeFilter & ( 1 << i ) ) != 0;
        bool bPropertiesMatch = ( memProperties.memoryTypes[ i ].propertyFlags & properties ) == properties;

        if ( bTypeMatch && bPropertiesMatch )
        {
            return i;
        }
    }

    throw B33_EXCEPT( "Failed to find suitable memory type!" );
}

} // namespace B33::Rendering
