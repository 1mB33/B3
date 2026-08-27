#include "B33Rendering.hpp"

#include "2DSprites/SpritesPipeline.hpp"
#include "Vulkan/Buffers/ImgBuffer.hpp"
#include "Vulkan/ErrorHandling.hpp"
#include "Vulkan/FrameResources.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/Utility.hpp"
#include "vulkan/vulkan_core.h"

namespace B33::Rendering
{

using namespace ::std;
using namespace ::B33::Math;

struct alignas( 16 ) Vertex
{
    float X;
    float Y;
};

const array<Vertex, 6> g_UnitQuadVertices = {
    Vertex( -1.0f, -1.f ),
    Vertex( 1.f, -1.f ),
    Vertex( -1.f, 1.f ),
    Vertex( 1.f, -1.f ),
    Vertex( 1.f, 1.f ),
    Vertex( -1.f, 1.f ),
};

// ---------------------------------------------------------------------------------------------------------------------
SpritesPipeline::~SpritesPipeline()
{
    if ( m_FragShader != VK_NULL_HANDLE )
    {
        vkDestroyShaderModule( GetAdaterInternal()->GetAdapterHandle(), m_FragShader, NULL );
    }

    m_FragShader = VK_NULL_HANDLE;

    if ( m_VertexShader != VK_NULL_HANDLE )
    {
        vkDestroyShaderModule( GetAdaterInternal()->GetAdapterHandle(), m_VertexShader, NULL );
    }

    m_VertexShader = VK_NULL_HANDLE;
}

// Public // -----------------------------------------------------------------------------------------------------------
void SpritesPipeline::CreatePipelineResourcesImpl()
{
    auto swapChain                    = GetSwapChainInternal();
    auto extent                       = swapChain->GetExtent();
    m_uCurFrame                       = 0;
    constexpr size_t unitVerticesSize = sizeof( Vertex ) * g_UnitQuadVertices.size();
    constexpr size_t instanceSize     = sizeof( SpriteData ) * MAX_SPRITES;

    m_pStageQuadBuffer = GetMemoryInternal()->ReserveStagingBuffer( unitVerticesSize );
    m_pQuadBuffer      = GetMemoryInternal()->ReserveVertexBuffer( unitVerticesSize );

    GetMemoryInternal()->UploadToStreamBufferRaw( g_UnitQuadVertices.data(), unitVerticesSize, m_pStageQuadBuffer );

    for ( uint32_t i = 0; i < Frame::MAX_FRAMES_IN_FLIGHT; ++i )
    {
        auto img = GetMemoryInternal()->ReserveImage( extent.width,
                                                      extent.height,
                                                      VK_FORMAT_D32_SFLOAT,
                                                      VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT );
        GetMemoryInternal()->ReserveImageView( img, VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT );

        m_PerFrameResources.push_back( {
            .uLastUploadedGeneration = ~(uint32_t)0,
            .bPendingGpuCopy         = false,
            .SpriteInstances         = GetMemoryInternal()->ReserveGPUBuffer( instanceSize ),
            .StageSpriteInstances    = GetMemoryInternal()->ReserveStagingBuffer( instanceSize ),
            .DepthImg                = std::move( img ),
            .DescSet                 = CreateDescriptorSet(),
        } );
    }
}

// --------------------------------------------------------------------------------------------------------------------
void SpritesPipeline::Update()
{
    auto &curFrame = m_PerFrameResources[ m_uCurFrame ];

    GetMemoryInternal()->UploadToStreamBufferDescSet(
        m_SpriteData.data(),
        m_SpriteData.size() * sizeof( SpriteData ),
        GetUniformUploadDescriptor( curFrame.StageSpriteInstances, EShaderResource::SpriteInstances ) );
}

// --------------------------------------------------------------------------------------------------------------------
void SpritesPipeline::RecordCommands( VkCommandBuffer        &cmdBuffer,
                                      VkPipelineStageFlagBits lastStage,
                                      VkImageLayout           lastLayout )
{
    auto  swapChain = GetSwapChainInternal();
    auto  image     = swapChain->GetCurrentImage();
    auto  imageView = swapChain->GetCurrentImageView();
    auto  extent    = swapChain->GetExtent();
    auto &curFrame  = m_PerFrameResources[ m_uCurFrame ];

    VkBufferCopy copyRegion = {
        .srcOffset = 0,
        .dstOffset = 0,
        .size      = m_pStageQuadBuffer->GetSizeInBytes(),
    };
    vkCmdCopyBuffer( cmdBuffer,
                     m_pStageQuadBuffer->GetBufferHandle(),
                     m_pQuadBuffer->GetBufferHandle(),
                     1,
                     &copyRegion );

    VkBufferMemoryBarrier buffersBarrier = {};
    buffersBarrier.sType                 = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    buffersBarrier.pNext                 = NULL;
    buffersBarrier.srcAccessMask         = VK_ACCESS_TRANSFER_WRITE_BIT;
    buffersBarrier.dstAccessMask         = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
    buffersBarrier.srcQueueFamilyIndex   = VK_QUEUE_FAMILY_IGNORED;
    buffersBarrier.dstQueueFamilyIndex   = VK_QUEUE_FAMILY_IGNORED;
    buffersBarrier.buffer                = m_pQuadBuffer->GetBufferHandle();
    buffersBarrier.offset                = 0;
    buffersBarrier.size                  = VK_WHOLE_SIZE;

    vkCmdPipelineBarrier( cmdBuffer,
                          VK_PIPELINE_STAGE_TRANSFER_BIT,
                          VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
                          0,
                          0,
                          NULL,
                          1,
                          &buffersBarrier,
                          0,
                          NULL );

    VkBufferCopy copyRegion2 = {
        .srcOffset = 0,
        .dstOffset = 0,
        .size      = curFrame.StageSpriteInstances->GetSizeInBytes(),
    };
    vkCmdCopyBuffer( cmdBuffer,
                     curFrame.StageSpriteInstances->GetBufferHandle(),
                     curFrame.SpriteInstances->GetBufferHandle(),
                     1,
                     &copyRegion2 );

    VkBufferMemoryBarrier imgMemBarrier = {};
    imgMemBarrier.sType                 = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    imgMemBarrier.pNext                 = NULL;
    imgMemBarrier.srcAccessMask         = VK_ACCESS_TRANSFER_WRITE_BIT;
    imgMemBarrier.dstAccessMask         = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
    imgMemBarrier.srcQueueFamilyIndex   = VK_QUEUE_FAMILY_IGNORED;
    imgMemBarrier.dstQueueFamilyIndex   = VK_QUEUE_FAMILY_IGNORED;
    imgMemBarrier.buffer                = curFrame.SpriteInstances->GetBufferHandle();
    imgMemBarrier.offset                = 0;
    imgMemBarrier.size                  = VK_WHOLE_SIZE;

    vkCmdPipelineBarrier( cmdBuffer,
                          VK_PIPELINE_STAGE_TRANSFER_BIT,
                          VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
                          0,
                          0,
                          NULL,
                          1,
                          &imgMemBarrier,
                          0,
                          NULL );

    lastStage = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;

    VkImageMemoryBarrier toColorAttachment = {};
    toColorAttachment.sType                = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    toColorAttachment.srcAccessMask        = lastStage;
    toColorAttachment.dstAccessMask        = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    toColorAttachment.oldLayout            = lastLayout;
    toColorAttachment.newLayout            = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    toColorAttachment.srcQueueFamilyIndex  = VK_QUEUE_FAMILY_IGNORED;
    toColorAttachment.dstQueueFamilyIndex  = VK_QUEUE_FAMILY_IGNORED;
    toColorAttachment.image                = image;
    toColorAttachment.subresourceRange     = VkImageSubresourceRange { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

    VkClearValue cv = {};
    cv.color        = VkClearColorValue { { 0.f, 0.f, 0.3f, 1.f } };

    VkRenderingAttachmentInfo colorAttachment = {};
    colorAttachment.sType                     = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    colorAttachment.imageView                 = imageView;
    colorAttachment.imageLayout               = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachment.loadOp                    = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.clearValue                = cv;
    colorAttachment.storeOp                   = VK_ATTACHMENT_STORE_OP_STORE;

    VkClearValue depthClear       = {};
    depthClear.depthStencil.depth = 1.0f;

    VkRenderingAttachmentInfo depthAttachment = {};
    depthAttachment.sType                     = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    depthAttachment.imageView                 = curFrame.DepthImg.GetImageView();
    depthAttachment.imageLayout               = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    depthAttachment.resolveMode               = VK_RESOLVE_MODE_NONE;
    depthAttachment.loadOp                    = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp                   = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.clearValue                = depthClear;

    VkImageMemoryBarrier toDepthAttachment = {};
    toDepthAttachment.sType                = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    toDepthAttachment.srcAccessMask        = 0;
    toDepthAttachment.dstAccessMask =
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
    toDepthAttachment.oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
    toDepthAttachment.newLayout           = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    toDepthAttachment.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    toDepthAttachment.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    toDepthAttachment.image               = curFrame.DepthImg.GetImage();
    toDepthAttachment.subresourceRange    = VkImageSubresourceRange { VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1 };

    VkImageMemoryBarrier attachmentBarriers[] = { toColorAttachment, toDepthAttachment };

    vkCmdPipelineBarrier( cmdBuffer,
                          lastStage,
                          VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                          0,
                          0,
                          NULL,
                          0,
                          NULL,
                          2,
                          attachmentBarriers );

    B33_TRACE( L"Extent w%d h%d", extent.width, extent.height );

    VkRenderingInfo renderingInfo      = {};
    renderingInfo.sType                = VK_STRUCTURE_TYPE_RENDERING_INFO;
    renderingInfo.renderArea           = VkRect2D { { 0, 0 }, extent };
    renderingInfo.layerCount           = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments    = &colorAttachment;
    renderingInfo.pDepthAttachment     = &depthAttachment;

    vkCmdBeginRendering( cmdBuffer, &renderingInfo );

    vkCmdBindPipeline( cmdBuffer, this->GetPipelineBindPoint(), this->GetPipelineHandle() );

    VkViewport viewport = { 0.0f, 0.0f, float( extent.width ), float( extent.height ), 0.0f, 1.0f };
    VkRect2D   scissor  = { { 0, 0 }, extent };
    vkCmdSetViewport( cmdBuffer, 0, 1, &viewport );
    vkCmdSetScissor( cmdBuffer, 0, 1, &scissor );

    vkCmdBindDescriptorSets( cmdBuffer,
                             this->GetPipelineBindPoint(),
                             this->GetLayoutHandle(),
                             0,
                             1,
                             &curFrame.DescSet,
                             0,
                             NULL );

    vkCmdPushConstants( cmdBuffer,
                        GetLayoutHandle(),
                        VK_SHADER_STAGE_VERTEX_BIT,
                        0,
                        sizeof( SpritesPushConstants ),
                        &m_Vpc );

    VkBuffer     vertexBuffers[] = { m_pQuadBuffer->GetBufferHandle() };
    VkDeviceSize offsets[]       = { 0 };
    vkCmdBindVertexBuffers( cmdBuffer, 0, 1, vertexBuffers, offsets );

    const uint32_t spriteCount =
        static_cast<uint32_t>( min( m_SpriteData.size(), static_cast<size_t>( MAX_SPRITES ) ) );

    B33_TRACE( L"Drawing %d sprites", spriteCount );

    if ( spriteCount > 0 )
    {
        vkCmdDraw( cmdBuffer, static_cast<uint32_t>( g_UnitQuadVertices.size() ), spriteCount, 0, 0 );
    }

    vkCmdEndRendering( cmdBuffer );

    VkImageMemoryBarrier presentBarrier = {};
    presentBarrier.sType                = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    presentBarrier.srcAccessMask        = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    presentBarrier.dstAccessMask        = 0;
    presentBarrier.oldLayout            = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    presentBarrier.newLayout            = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    presentBarrier.srcQueueFamilyIndex  = VK_QUEUE_FAMILY_IGNORED;
    presentBarrier.dstQueueFamilyIndex  = VK_QUEUE_FAMILY_IGNORED;
    presentBarrier.image                = image;
    presentBarrier.subresourceRange     = {
        .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel   = 0,
        .levelCount     = 1,
        .baseArrayLayer = 0,
        .layerCount     = 1,
    };

    vkCmdPipelineBarrier( cmdBuffer,
                          VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                          VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                          0,
                          0,
                          NULL,
                          0,
                          NULL,
                          1,
                          &presentBarrier );
}

// --------------------------------------------------------------------------------------------------------------------
void SpritesPipeline::Reset()
{
    auto swapChain = GetSwapChainInternal();
    auto extent    = swapChain->GetExtent();

    for ( auto &resources : m_PerFrameResources )
    {
        auto img = GetMemoryInternal()->ReserveImage( extent.width,
                                                      extent.height,
                                                      VK_FORMAT_D32_SFLOAT,
                                                      VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT );
        GetMemoryInternal()->ReserveImageView( img, VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT );

        resources.DepthImg = std::move( img );
    }
}

// Private // ----------------------------------------------------------------------------------------------------------
VkDescriptorSetLayout SpritesPipeline::CreateDescriptorLayoutImpl()
{
    array<VkDescriptorSetLayoutBinding, 1> bindings = {};
    VkDescriptorSetLayout                  descriptorSetLayout;

    bindings[ 0 ]                 = {};
    bindings[ 0 ].binding         = EShaderResource::SpriteInstances;
    bindings[ 0 ].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[ 0 ].descriptorCount = 1;
    bindings[ 0 ].stageFlags      = VK_SHADER_STAGE_VERTEX_BIT;

    array<VkDescriptorBindingFlags, 1> bindingFlags = { VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT };

    VkDescriptorSetLayoutBindingFlagsCreateInfo bindingCreateInfo = {};
    bindingCreateInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
    bindingCreateInfo.bindingCount  = bindingFlags.size();
    bindingCreateInfo.pBindingFlags = bindingFlags.data();

    VkDescriptorSetLayoutCreateInfo layoutCreateInfo = {};
    layoutCreateInfo.sType                           = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutCreateInfo.pNext                           = &bindingCreateInfo;
    layoutCreateInfo.bindingCount                    = bindings.size();
    layoutCreateInfo.pBindings                       = bindings.data();
    layoutCreateInfo.flags                           = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;

    THROW_IF_FAILED( vkCreateDescriptorSetLayout( GetAdaterInternal()->GetAdapterHandle(),
                                                  &layoutCreateInfo,
                                                  NULL,
                                                  &descriptorSetLayout ) );

    return descriptorSetLayout;
}

// --------------------------------------------------------------------------------------------------------------------
UploadDescriptor SpritesPipeline::GetUniformUploadDescriptor( const shared_ptr<GPUStreamBuffer> &outBuffer,
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
VkDescriptorPool SpritesPipeline::CreateDescriptorPoolImpl()
{
    const array<VkDescriptorPoolSize, 1> poolSizes = {
        VkDescriptorPoolSize( VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, Frame::MAX_FRAMES_IN_FLIGHT ),
    };
    VkDescriptorPool descriptorPool;

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType                      = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.maxSets                    = Frame::MAX_FRAMES_IN_FLIGHT;
    poolInfo.poolSizeCount              = poolSizes.size();
    poolInfo.pPoolSizes                 = poolSizes.data();
    poolInfo.flags                      = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;

    THROW_IF_FAILED(
        vkCreateDescriptorPool( GetAdaterInternal()->GetAdapterHandle(), &poolInfo, NULL, &descriptorPool ) );

    return descriptorPool;
}

// ---------------------------------------------------------------------------------------------------------------------
VkDescriptorSet SpritesPipeline::CreateDescriptorSet()
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
VkPipelineLayout SpritesPipeline::CreatePipelineLayoutImpl()
{
    VkPipelineLayout      pipelineLayout;
    VkDescriptorSetLayout descLayout = GetDescriptorLayoutInternal();

    VkPushConstantRange pushConstantRange = {
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .offset     = 0,
        .size       = sizeof( SpritesPushConstants ),
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
VkPipeline SpritesPipeline::CreatePipelineImpl()
{
    const VkDevice device   = GetAdaterInternal()->GetAdapterHandle();
    VkPipeline     pipeline = VK_NULL_HANDLE;
    m_VertexShader =
        Shaders::LoadShader( ::B33::App::AppResources::Get().GetExecutablePathA() + "/../Assets/Shaders/2DSpritesV.spv",
                             GetAdaterInternal().get() );

    m_FragShader =
        Shaders::LoadShader( ::B33::App::AppResources::Get().GetExecutablePathA() + "/../Assets/Shaders/2DSpritesF.spv",
                             GetAdaterInternal().get() );

    array<VkPipelineShaderStageCreateInfo, 2> stages = {};

    stages[ 0 ].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[ 0 ].stage  = VK_SHADER_STAGE_VERTEX_BIT;
    stages[ 0 ].module = m_VertexShader;
    stages[ 0 ].pName  = "main";

    stages[ 1 ].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[ 1 ].stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
    stages[ 1 ].module = m_FragShader;
    stages[ 1 ].pName  = "main";

    VkVertexInputBindingDescription binding = {};
    binding.binding                         = 0;
    binding.stride                          = sizeof( Vertex );
    binding.inputRate                       = VK_VERTEX_INPUT_RATE_VERTEX;

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
    inputAssemblyState.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyState.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyState.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType                             = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount                     = 1;
    viewportState.scissorCount                      = 1;

    VkVertexInputAttributeDescription attribute = {};
    attribute.location                          = 0;
    attribute.binding                           = 0;
    attribute.format                            = VK_FORMAT_R32G32_SFLOAT;
    attribute.offset                            = 0;

    VkPipelineVertexInputStateCreateInfo vertexInputState = {};
    vertexInputState.sType                                = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputState.vertexBindingDescriptionCount        = 1;
    vertexInputState.pVertexBindingDescriptions           = &binding;
    vertexInputState.vertexAttributeDescriptionCount      = 1;
    vertexInputState.pVertexAttributeDescriptions         = &attribute;

    array<VkDynamicState, 2>         dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineDynamicStateCreateInfo dynamicState  = {};
    dynamicState.sType                             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount                 = static_cast<uint32_t>( dynamicStates.size() );
    dynamicState.pDynamicStates                    = dynamicStates.data();

    VkPipelineRasterizationStateCreateInfo rasterizationState = {};
    rasterizationState.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.depthClampEnable        = VK_FALSE;
    rasterizationState.rasterizerDiscardEnable = VK_FALSE;
    rasterizationState.polygonMode             = VK_POLYGON_MODE_FILL;
    rasterizationState.cullMode                = VK_CULL_MODE_NONE;
    rasterizationState.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizationState.depthBiasEnable         = VK_FALSE;
    rasterizationState.lineWidth               = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisampleState = {};
    multisampleState.sType                                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.rasterizationSamples                 = VK_SAMPLE_COUNT_1_BIT;
    multisampleState.sampleShadingEnable                  = VK_FALSE;

    VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
    depthStencilState.sType            = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilState.depthTestEnable  = VK_TRUE;
    depthStencilState.depthWriteEnable = VK_TRUE;
    depthStencilState.depthCompareOp   = VK_COMPARE_OP_LESS;

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.blendEnable                         = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor                 = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor                 = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp                        = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor                 = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor                 = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp                        = VK_BLEND_OP_ADD;
    colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo colorBlendState = {};
    colorBlendState.sType                               = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.logicOpEnable                       = VK_FALSE;
    colorBlendState.attachmentCount                     = 1;
    colorBlendState.pAttachments                        = &colorBlendAttachment;

    VkPipelineRenderingCreateInfo renderingInfo = {};
    renderingInfo.sType                         = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
    renderingInfo.colorAttachmentCount          = 1;
    renderingInfo.pColorAttachmentFormats       = &Swapchain::TargetedFormat;
    renderingInfo.depthAttachmentFormat         = VK_FORMAT_D32_SFLOAT;

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType                        = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext                        = &renderingInfo;
    pipelineInfo.stageCount                   = static_cast<uint32_t>( stages.size() );
    pipelineInfo.pStages                      = stages.data();
    pipelineInfo.pVertexInputState            = &vertexInputState;
    pipelineInfo.pInputAssemblyState          = &inputAssemblyState;
    pipelineInfo.pViewportState               = &viewportState;
    pipelineInfo.pRasterizationState          = &rasterizationState;
    pipelineInfo.pMultisampleState            = &multisampleState;
    pipelineInfo.pDepthStencilState           = &depthStencilState;
    pipelineInfo.pColorBlendState             = &colorBlendState;
    pipelineInfo.pDynamicState                = &dynamicState;
    pipelineInfo.layout                       = this->GetLayoutHandle();

    THROW_IF_FAILED( vkCreateGraphicsPipelines( device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &pipeline ) );

    return pipeline;
}

} // namespace B33::Rendering
