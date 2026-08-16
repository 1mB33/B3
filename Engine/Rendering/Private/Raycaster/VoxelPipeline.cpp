#include "B33Core.h"
#include "B33Rendering.hpp"

#include "Raycaster/VoxelPipeline.hpp"
#include "Vulkan/ErrorHandling.hpp"
#include "Vulkan/Buffers/GPUStreamBuffer.hpp"
#include "Vulkan/FrameResources.hpp"
#include "Vulkan/Memory/MemoryUploadTracker.hpp"
#include "Vulkan/Utility.hpp"

namespace B33::Rendering
{

using namespace ::std;
using namespace ::B33::Math;

// ---------------------------------------------------------------------------------------------------------------------
VoxelPipeline::~VoxelPipeline()
{
    if ( m_ShaderModule != VK_NULL_HANDLE )
    {
        vkDestroyShaderModule( GetAdaterInternal()->GetAdapterHandle(), m_ShaderModule, NULL );
    }

    m_ShaderModule = VK_NULL_HANDLE;
}

// Public // -----------------------------------------------------------------------------------------------------------
void VoxelPipeline::CreatePipelineResourcesImpl( ::std::shared_ptr<::B33::Rendering::CubeWorld> pWorld )
{
    m_uCurFrame  = 0;
    m_pVoxelGrid = pWorld;

    for ( uint32_t i = 0; i < Frame::MAX_FRAMES_IN_FLIGHT; ++i )
    {
        m_PerFrameResources.push_back( {
            .pVoxelBuffer     = GetMemoryInternal()->ReserveGPUBuffer( pWorld->GetVoxelsSizeInBytes() ),
            .pPositionsBuffer = GetMemoryInternal()->ReserveGPUBuffer(
                pWorld->GetStoredObjects().GetPositions().capacity() * sizeof( Vec3 ) ),
            .pRotationsBuffer = GetMemoryInternal()->ReserveGPUBuffer(
                pWorld->GetStoredObjects().GetRotations().capacity() * sizeof( Vec3 ) ),
            .pHalfSizesBuffer = GetMemoryInternal()->ReserveGPUBuffer(
                ( /*FIXME: */ (Cubes &)pWorld->GetStoredObjects() ).GetHalfSizes().capacity() * sizeof( Vec3 ) ),
            .pStageVoxelBuffer    = GetMemoryInternal()->ReserveStagingBuffer( pWorld->GetVoxelsSizeInBytes() ),
            .pStagePositonsBuffer = GetMemoryInternal()->ReserveStagingBuffer(
                pWorld->GetStoredObjects().GetPositions().capacity() * sizeof( Vec3 ) ),
            .pStageRotationsBuffer = GetMemoryInternal()->ReserveStagingBuffer(
                pWorld->GetStoredObjects().GetRotations().capacity() * sizeof( Vec3 ) ),
            .pStageHalfSizesBuffer = GetMemoryInternal()->ReserveStagingBuffer(
                ( /*FIXME: */ (Cubes &)pWorld->GetStoredObjects() ).GetHalfSizes().capacity() * sizeof( Vec3 ) ),
            .uStorageBuffersFlags     = 0,
            .uLastStorageBuffersFlags = 0,
            .DescSet                  = CreateDescriptorSet(),
        } );
    }
}

// --------------------------------------------------------------------------------------------------------------------
void VoxelPipeline::Update()
{
    const auto lastFrameIndex = m_uCurFrame;
    auto      &lastPerFrame   = m_PerFrameResources[ lastFrameIndex ];
    m_uCurFrame               = ( m_uCurFrame + 1 ) % Frame::MAX_FRAMES_IN_FLIGHT;
    auto &curPerFrame         = m_PerFrameResources[ m_uCurFrame ];

    curPerFrame.uStorageBuffersFlags = lastPerFrame.uStorageBuffersFlags & ~curPerFrame.uLastStorageBuffersFlags;
    lastPerFrame.uStorageBuffersFlags &= 0;

    curPerFrame.uStorageBuffersFlags |= m_pVoxelGrid->GetChanged();
    curPerFrame.uLastStorageBuffersFlags = curPerFrame.uStorageBuffersFlags;

    if ( m_pVoxelGrid->ReuploadStatus() & RequestStaging )
    {
        B33_TRACE( L"Setting flags on RequestStaging" );
        curPerFrame.uStorageBuffersFlags = ( EGridChanged::HalfSize << 1 ) - 1;
    }
    if ( !curPerFrame.uStorageBuffersFlags )
    {
        B33_TRACE( L"Checking reupload status but got %d", curPerFrame.uStorageBuffersFlags );
        return;
    }
    B33_TRACE( L"Staging on GPU" );
    if ( curPerFrame.uStorageBuffersFlags )
    {
        GetMemoryInternal()->UploadOnStreamBuffer(
            m_pVoxelGrid->GetGrid().data(),
            m_pVoxelGrid->GetGrid().size() * sizeof( Voxel ),
            GetUniformUploadDescriptor( curPerFrame.pStageVoxelBuffer, VoxelPipeline::EShaderResource::VoxelGrid ) );
    }

    if ( curPerFrame.uStorageBuffersFlags & EGridChanged::Position )
    {
        GetMemoryInternal()->UploadOnStreamBuffer(
            m_pVoxelGrid->GetStoredObjects().GetPositions().data(),
            m_pVoxelGrid->GetStoredObjects().GetPositions().size() * sizeof( Vec3 ),
            GetUniformUploadDescriptor( curPerFrame.pStagePositonsBuffer,
                                        VoxelPipeline::EShaderResource::ObjectPositions ) );
    }

    if ( curPerFrame.uStorageBuffersFlags & EGridChanged::Rotation )
    {
        GetMemoryInternal()->UploadOnStreamBuffer(
            m_pVoxelGrid->GetStoredObjects().GetRotations().data(),
            m_pVoxelGrid->GetStoredObjects().GetRotations().size() * sizeof( Vec3 ),
            GetUniformUploadDescriptor( curPerFrame.pStageRotationsBuffer,
                                        VoxelPipeline::EShaderResource::ObjectRotations ) );
    }

    if ( curPerFrame.uStorageBuffersFlags & EGridChanged::HalfSize )
    {
        GetMemoryInternal()->UploadOnStreamBuffer(
            ( static_cast<const Cubes &>( m_pVoxelGrid->GetStoredObjects() ) ).GetHalfSizes().data(),
            ( static_cast<const Cubes &>( m_pVoxelGrid->GetStoredObjects() ) ).GetHalfSizes().size() * sizeof( Vec3 ),
            GetUniformUploadDescriptor( curPerFrame.pStageHalfSizesBuffer,
                                        VoxelPipeline::EShaderResource::ObjectHalfSizes ) );
    }
}

// --------------------------------------------------------------------------------------------------------------------
void VoxelPipeline::RecordCommands( VkCommandBuffer        &cmdBuffer,
                                    VkPipelineStageFlagBits lastStage,
                                    VkImageLayout           lastLayout )
{
    auto  swapChain   = GetSwapChainInternal();
    auto  image       = swapChain->GetImage();
    auto  imageView   = swapChain->GetImageView();
    auto &curPerFrame = m_PerFrameResources[ m_uCurFrame ];

    this->LoadImage( imageView );

    vkCmdBindPipeline( cmdBuffer, this->GetPipelineBindPoint(), this->GetPipelineHandle() );

    VkImageMemoryBarrier barrier = {};
    barrier.sType                = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.srcAccessMask        = 0;
    barrier.dstAccessMask        = VK_ACCESS_SHADER_WRITE_BIT;
    barrier.oldLayout            = lastLayout;
    barrier.newLayout            = VK_IMAGE_LAYOUT_GENERAL;
    barrier.srcQueueFamilyIndex  = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex  = VK_QUEUE_FAMILY_IGNORED;
    barrier.image                = image;
    barrier.subresourceRange     = VkImageSubresourceRange { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

    vkCmdPipelineBarrier( cmdBuffer, lastStage, this->GetPipelineStageFlagBits(), 0, 0, NULL, 0, NULL, 1, &barrier );

    vkCmdBindDescriptorSets( cmdBuffer,
                             this->GetPipelineBindPoint(),
                             this->GetLayoutHandle(),
                             0,
                             1,
                             &curPerFrame.DescSet,
                             0,
                             NULL );

    vkCmdPushConstants( cmdBuffer,
                        GetLayoutHandle(),
                        VK_SHADER_STAGE_COMPUTE_BIT,
                        0,
                        sizeof( VoxelPushConstants ),
                        &m_Vpc );

    if ( curPerFrame.uStorageBuffersFlags )
    {
        B33_TRACE( L"Uploading on GPU" );

        VkBufferCopy copyRegion = {
            .srcOffset = 0,
            .dstOffset = 0,
            .size      = curPerFrame.pStageVoxelBuffer->GetSizeInBytes(),
        };
        vkCmdCopyBuffer( cmdBuffer,
                         curPerFrame.pStageVoxelBuffer->GetBufferHandle(),
                         curPerFrame.pVoxelBuffer->GetBufferHandle(),
                         1,
                         &copyRegion );

        if ( curPerFrame.uStorageBuffersFlags & EGridChanged::Position )
        {
            copyRegion.size = m_pVoxelGrid->GetStoredObjects().GetPositions().size() * sizeof( Vec3 );
            vkCmdCopyBuffer( cmdBuffer,
                             curPerFrame.pStagePositonsBuffer->GetBufferHandle(),
                             curPerFrame.pPositionsBuffer->GetBufferHandle(),
                             1,
                             &copyRegion );
        }

        if ( curPerFrame.uStorageBuffersFlags & EGridChanged::Rotation )
        {
            copyRegion.size = m_pVoxelGrid->GetStoredObjects().GetRotations().size() * sizeof( Vec3 );
            vkCmdCopyBuffer( cmdBuffer,
                             curPerFrame.pStageRotationsBuffer->GetBufferHandle(),
                             curPerFrame.pRotationsBuffer->GetBufferHandle(),
                             1,
                             &copyRegion );
        }

        if ( curPerFrame.uStorageBuffersFlags & EGridChanged::HalfSize )
        {
            copyRegion.size = ( static_cast<const Cubes &>( m_pVoxelGrid->GetStoredObjects() ) ).GetHalfSizes().size() *
                              sizeof( Vec3 );
            vkCmdCopyBuffer( cmdBuffer,
                             curPerFrame.pStageHalfSizesBuffer->GetBufferHandle(),
                             curPerFrame.pHalfSizesBuffer->GetBufferHandle(),
                             1,
                             &copyRegion );
        }

        vector<VkMappedMemoryRange>   mmrs           = {};
        vector<VkBufferMemoryBarrier> bufferBarriers = {};

        VkMappedMemoryRange mmr = {};
        mmr.sType               = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mmr.memory              = curPerFrame.pVoxelBuffer->GetMemoryHandle();
        mmr.offset              = 0;
        mmr.size                = VK_WHOLE_SIZE;

        mmrs.push_back( mmr );

        bufferBarriers.push_back( {
            .sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
            .pNext               = NULL,
            .srcAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT,
            .dstAccessMask       = VK_ACCESS_SHADER_READ_BIT,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .buffer              = curPerFrame.pVoxelBuffer->GetBufferHandle(),
            .offset              = 0,
            .size                = VK_WHOLE_SIZE,
        } );

        if ( curPerFrame.uStorageBuffersFlags & EGridChanged::Position )
        {
            VkMappedMemoryRange mmr2 = mmr;
            mmr2.memory              = curPerFrame.pPositionsBuffer->GetMemoryHandle();
            mmrs.push_back( mmr2 );
            bufferBarriers.push_back( {
                .sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
                .pNext               = NULL,
                .srcAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT,
                .dstAccessMask       = VK_ACCESS_SHADER_READ_BIT,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .buffer              = curPerFrame.pPositionsBuffer->GetBufferHandle(),
                .offset              = 0,
                .size                = VK_WHOLE_SIZE,
            } );
        }
        if ( curPerFrame.uStorageBuffersFlags & EGridChanged::Rotation )
        {
            VkMappedMemoryRange mmr3 = mmr;
            mmr3.memory              = curPerFrame.pRotationsBuffer->GetMemoryHandle();
            mmrs.push_back( mmr3 );
            bufferBarriers.push_back( {
                .sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
                .pNext               = NULL,
                .srcAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT,
                .dstAccessMask       = VK_ACCESS_SHADER_READ_BIT,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .buffer              = curPerFrame.pRotationsBuffer->GetBufferHandle(),
                .offset              = 0,
                .size                = VK_WHOLE_SIZE,
            } );
        }
        if ( curPerFrame.uStorageBuffersFlags & EGridChanged::HalfSize )
        {
            VkMappedMemoryRange mmr4 = mmr;
            mmr4.memory              = curPerFrame.pHalfSizesBuffer->GetMemoryHandle();
            mmrs.push_back( mmr4 );
            bufferBarriers.push_back( {
                .sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
                .pNext               = NULL,
                .srcAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT,
                .dstAccessMask       = VK_ACCESS_SHADER_READ_BIT,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .buffer              = curPerFrame.pHalfSizesBuffer->GetBufferHandle(),
                .offset              = 0,
                .size                = VK_WHOLE_SIZE,
            } );
        }

        vkCmdPipelineBarrier( cmdBuffer,
                              VK_PIPELINE_STAGE_TRANSFER_BIT,
                              VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                              0,
                              0,
                              NULL,
                              bufferBarriers.size(),
                              bufferBarriers.data(),
                              0,
                              NULL );
    }

    const uint32_t groupCountX = ( GetWindowDescInternal()->Data.Width + 31 ) >> 5;
    const uint32_t groupCountY = ( GetWindowDescInternal()->Data.Height + 7 ) >> 3;
    vkCmdDispatch( cmdBuffer, groupCountX, groupCountY, 1 );
}

// --------------------------------------------------------------------------------------------------------------------
void VoxelPipeline::Reset()
{
    for ( auto &perFrame : m_PerFrameResources )
    {
        if ( perFrame.pStageVoxelBuffer == nullptr || perFrame.pStagePositonsBuffer == nullptr ||
             perFrame.pStageHalfSizesBuffer == nullptr )
        {
            return;
        }

        perFrame.pStageVoxelBuffer->Reset();
        perFrame.pStagePositonsBuffer->Reset();
        perFrame.pStageRotationsBuffer->Reset();
        perFrame.pStageHalfSizesBuffer->Reset();
    }
    m_pVoxelGrid->ForceUpload();
}

// --------------------------------------------------------------------------------------------------------------------
UploadDescriptor VoxelPipeline::GetUniformUploadDescriptor( const shared_ptr<GPUStreamBuffer> &outBuffer,
                                                            const EShaderResource             &sr )
{
    VkDescriptorBufferInfo bufferInfo = {
        .buffer = outBuffer->GetBufferHandle(),
        .offset = 0,
        .range  = VK_WHOLE_SIZE,
    };

    VkWriteDescriptorSet write = {};
    write.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet               = m_PerFrameResources[ m_uCurFrame ].DescSet;
    write.dstBinding           = static_cast<uint32_t>( sr );
    write.descriptorCount      = 1;
    write.descriptorType       = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    write.pBufferInfo          = &bufferInfo;

    return UploadDescriptor( std::move( bufferInfo ), std::move( write ), EUploadType::StreamBuffer, outBuffer );
}

// ---------------------------------------------------------------------------------------------------------------------
void VoxelPipeline::LoadImage( VkImageView image )
{
    VkDescriptorImageInfo imageInfo = {};
    imageInfo.imageView             = image;
    imageInfo.imageLayout           = VK_IMAGE_LAYOUT_GENERAL;

    VkWriteDescriptorSet imageWrite = {};
    imageWrite.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    imageWrite.dstSet               = m_PerFrameResources[ m_uCurFrame ].DescSet;
    imageWrite.dstBinding           = 0;
    imageWrite.dstArrayElement      = 0;
    imageWrite.descriptorCount      = 1;
    imageWrite.descriptorType       = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    imageWrite.pImageInfo           = &imageInfo;

    vkUpdateDescriptorSets( GetAdaterInternal()->GetAdapterHandle(), 1, &imageWrite, 0, NULL );
}

// Private // ----------------------------------------------------------------------------------------------------------
VkDescriptorSetLayout VoxelPipeline::CreateDescriptorLayoutImpl()
{
    array<VkDescriptorSetLayoutBinding, 5> bindings = {};
    VkDescriptorSetLayout                  descriptorSetLayout;

    bindings[ 0 ]                 = {};
    bindings[ 0 ].binding         = 0;
    bindings[ 0 ].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    bindings[ 0 ].descriptorCount = 1;
    bindings[ 0 ].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;

    bindings[ 1 ]                 = {};
    bindings[ 1 ].binding         = VoxelPipeline::EShaderResource::VoxelGrid;
    bindings[ 1 ].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[ 1 ].descriptorCount = 1;
    bindings[ 1 ].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;

    bindings[ 2 ]                 = {};
    bindings[ 2 ].binding         = VoxelPipeline::EShaderResource::ObjectPositions;
    bindings[ 2 ].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[ 2 ].descriptorCount = 1;
    bindings[ 2 ].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;

    bindings[ 3 ]                 = {};
    bindings[ 3 ].binding         = VoxelPipeline::EShaderResource::ObjectRotations;
    bindings[ 3 ].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[ 3 ].descriptorCount = 1;
    bindings[ 3 ].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;

    bindings[ 4 ]                 = {};
    bindings[ 4 ].binding         = VoxelPipeline::EShaderResource::ObjectHalfSizes;
    bindings[ 4 ].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[ 4 ].descriptorCount = 1;
    bindings[ 4 ].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;

    VkDescriptorSetLayoutCreateInfo layoutCreateInfo = {};
    layoutCreateInfo.sType                           = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutCreateInfo.bindingCount                    = static_cast<uint32_t>( bindings.size() );
    layoutCreateInfo.pBindings                       = &bindings[ 0 ];

    THROW_IF_FAILED( vkCreateDescriptorSetLayout( GetAdaterInternal()->GetAdapterHandle(),
                                                  &layoutCreateInfo,
                                                  NULL,
                                                  &descriptorSetLayout ) );

    return descriptorSetLayout;
}

// ---------------------------------------------------------------------------------------------------------------------
VkDescriptorPool VoxelPipeline::CreateDescriptorPoolImpl()
{
    const vector<VkDescriptorPoolSize> poolSizes = {
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, Frame::MAX_FRAMES_IN_FLIGHT * 1 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, Frame::MAX_FRAMES_IN_FLIGHT * 4 },
    };

    VkDescriptorPool descriptorPool;

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType                      = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.maxSets                    = Frame::MAX_FRAMES_IN_FLIGHT;
    poolInfo.poolSizeCount              = static_cast<uint32_t>( poolSizes.size() );
    poolInfo.pPoolSizes                 = &poolSizes[ 0 ];

    THROW_IF_FAILED(
        vkCreateDescriptorPool( GetAdaterInternal()->GetAdapterHandle(), &poolInfo, NULL, &descriptorPool ) );

    return descriptorPool;
}

// ---------------------------------------------------------------------------------------------------------------------
VkDescriptorSet VoxelPipeline::CreateDescriptorSet()
{
    VkDescriptorSetLayout descLayout = GetDescriptorLayoutInternal();
    VkDescriptorSet       descriptorSet;

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType                       = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool              = GetDescriptorPoolInternal();
    allocInfo.descriptorSetCount          = 1;
    allocInfo.pSetLayouts                 = &descLayout;

    THROW_IF_FAILED( vkAllocateDescriptorSets( GetAdaterInternal()->GetAdapterHandle(), &allocInfo, &descriptorSet ) );

    return descriptorSet;
}

// ---------------------------------------------------------------------------------------------------------------------
VkPipelineLayout VoxelPipeline::CreatePipelineLayoutImpl()
{
    VkPipelineLayout      pipelineLayout;
    VkDescriptorSetLayout descLayout = GetDescriptorLayoutInternal();

    VkPushConstantRange pushConstantRange = {
        .stageFlags = VK_SHADER_STAGE_COMPUTE_BIT,
        .offset     = 0,
        .size       = sizeof( VoxelPushConstants ),
    };

    VkPipelineLayoutCreateInfo layoutInfo = {
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext                  = NULL,
        .flags                  = 0,
        .setLayoutCount         = 1,
        .pSetLayouts            = &descLayout,
        .pushConstantRangeCount = 1,
        .pPushConstantRanges    = &pushConstantRange,
    };

    THROW_IF_FAILED(
        vkCreatePipelineLayout( GetAdaterInternal()->GetAdapterHandle(), &layoutInfo, NULL, &pipelineLayout ) );

    return pipelineLayout;
}

// ---------------------------------------------------------------------------------------------------------------------
VkPipeline VoxelPipeline::CreatePipelineImpl()
{
    const VkDevice device   = GetAdaterInternal()->GetAdapterHandle();
    VkPipeline     pipeline = VK_NULL_HANDLE;
    m_ShaderModule =
        Shaders::LoadShader( ::B33::App::AppResources::Get().GetExecutablePathA() + "/Assets/Shaders/Raycast.spv",
                             GetAdaterInternal().get() );

    VkPipelineShaderStageCreateInfo shaderStage = {};
    shaderStage.sType                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStage.stage                           = VK_SHADER_STAGE_COMPUTE_BIT;
    shaderStage.module                          = m_ShaderModule;
    shaderStage.pName                           = "main";

    VkComputePipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType                       = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.stage                       = shaderStage;
    pipelineInfo.layout                      = this->GetLayoutHandle();

    THROW_IF_FAILED( vkCreateComputePipelines( device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &pipeline ) );

    return pipeline;
}

} // namespace B33::Rendering
