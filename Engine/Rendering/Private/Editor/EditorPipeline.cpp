#include "B33Rendering.hpp"

#include "Editor/EditorPipeline.hpp"
#include "Vulkan/ErrorHandling.hpp"
#include "Vulkan/Buffers/GPUStreamBuffer.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/Utility.hpp"

namespace B33::Rendering
{

using namespace ::std;
using namespace ::B33::Math;

// Constructors // ----------------------------------------------------------------------------------------------------
EditorPipeline::EditorPipeline()
  : IPipeline( VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_IMAGE_LAYOUT_GENERAL, VK_PIPELINE_BIND_POINT_COMPUTE )
  , m_ShaderModule( VK_NULL_HANDLE )
{
}

// ---------------------------------------------------------------------------------------------------------------------
EditorPipeline::~EditorPipeline()
{
    if ( GetAdaterInternal().get() && m_ImageView != VK_NULL_HANDLE )
    {
        vkDestroyImageView( GetAdaterInternal()->GetAdapterHandle(), m_ImageView, nullptr );
        m_ImageView = VK_NULL_HANDLE;
    }

    if ( GetAdaterInternal().get() && m_ShaderModule != VK_NULL_HANDLE )
    {
        vkDestroyShaderModule( GetAdaterInternal()->GetAdapterHandle(), m_ShaderModule, NULL );
        m_ShaderModule = VK_NULL_HANDLE;
    }
}

// Public // -----------------------------------------------------------------------------------------------------------
void EditorPipeline::CreatePipelineResourcesImpl() {}

// --------------------------------------------------------------------------------------------------------------------
void EditorPipeline::Update() {}

// --------------------------------------------------------------------------------------------------------------------
void EditorPipeline::RecordCommands( VkCommandBuffer                              &cmdBuffer,
                                     VkPipelineStageFlagBits                       lastStage,
                                     __B33_ATTRIBUTE_MIGHT_BE_UNUSED VkImageLayout lastLayout )
{
    auto swapChain = GetSwapChainInternal();

    this->LoadImage( swapChain->GetImage() );

    vkCmdBindPipeline( cmdBuffer, this->GetPipelineBindPoint(), this->GetPipelineHandle() );

    VkImageMemoryBarrier barrier = {};
    barrier.sType                = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.srcAccessMask        = 0;
    barrier.dstAccessMask        = VK_ACCESS_SHADER_WRITE_BIT;
    barrier.oldLayout            = lastLayout;
    barrier.newLayout            = VK_IMAGE_LAYOUT_GENERAL;
    barrier.srcQueueFamilyIndex  = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex  = VK_QUEUE_FAMILY_IGNORED;
    barrier.image                = swapChain->GetImage();
    barrier.subresourceRange     = VkImageSubresourceRange { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

    vkCmdPipelineBarrier( cmdBuffer, lastStage, this->GetPipelineStageFlagBits(), 0, 0, NULL, 0, NULL, 1, &barrier );

    vkCmdBindDescriptorSets( cmdBuffer,
                             this->GetPipelineBindPoint(),
                             this->GetLayoutHandle(),
                             0,
                             1,
                             &this->GetDescriptorSet(),
                             0,
                             NULL );

    vkCmdPushConstants( cmdBuffer,
                        GetLayoutHandle(),
                        VK_SHADER_STAGE_COMPUTE_BIT,
                        0,
                        sizeof( EditorPushConstants ),
                        &m_Vpc );

    vkCmdPipelineBarrier( cmdBuffer, lastStage, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, 0, NULL, 0, NULL, 0, NULL );

    const uint32_t groupCountX = ( GetWindowDescInternal()->Width + 31 ) >> 5;
    const uint32_t groupCountY = ( GetWindowDescInternal()->Height + 7 ) >> 3;
    vkCmdDispatch( cmdBuffer, groupCountX, groupCountY, 1 );
}

// --------------------------------------------------------------------------------------------------------------------
void EditorPipeline::Reset() {}

// ---------------------------------------------------------------------------------------------------------------------
void EditorPipeline::LoadImage( VkImage image )
{
    if ( m_ImageView != VK_NULL_HANDLE )
    {
        vkDestroyImageView( GetAdaterInternal()->GetAdapterHandle(), m_ImageView, nullptr );
        m_ImageView = VK_NULL_HANDLE;
    }

    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType                 = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image                 = image;
    viewInfo.viewType              = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format                = Swapchain::TargetedFormat;
    viewInfo.subresourceRange      = {
        .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel   = 0,
        .levelCount     = 1,
        .baseArrayLayer = 0,
        .layerCount     = 1,
    };

    THROW_IF_FAILED( vkCreateImageView( GetAdaterInternal()->GetAdapterHandle(), &viewInfo, NULL, &m_ImageView ) );

    VkDescriptorImageInfo imageInfo = {};
    imageInfo.imageView             = m_ImageView;
    imageInfo.imageLayout           = VK_IMAGE_LAYOUT_GENERAL;

    VkWriteDescriptorSet imageWrite = {};
    imageWrite.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    imageWrite.dstSet               = this->GetDescriptorSet();
    imageWrite.dstBinding           = 0;
    imageWrite.dstArrayElement      = 0;
    imageWrite.descriptorCount      = 1;
    imageWrite.descriptorType       = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    imageWrite.pImageInfo           = &imageInfo;

    vkUpdateDescriptorSets( GetAdaterInternal()->GetAdapterHandle(), 1, &imageWrite, 0, NULL );
}

// Private // ----------------------------------------------------------------------------------------------------------
VkDescriptorSetLayout EditorPipeline::CreateDescriptorLayoutImpl()
{
    array<VkDescriptorSetLayoutBinding, 1> bindings = {};
    VkDescriptorSetLayout                  descriptorSetLayout;

    bindings[ 0 ]                 = {};
    bindings[ 0 ].binding         = 0;
    bindings[ 0 ].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    bindings[ 0 ].descriptorCount = 1;
    bindings[ 0 ].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;

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
VkDescriptorPool EditorPipeline::CreateDescriptorPoolImpl()
{
    const vector<VkDescriptorPoolSize> poolSizes = {
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1 },
    };

    VkDescriptorPool descriptorPool;

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType                      = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.maxSets                    = 1;
    poolInfo.poolSizeCount              = static_cast<uint32_t>( poolSizes.size() );
    poolInfo.pPoolSizes                 = &poolSizes[ 0 ];

    THROW_IF_FAILED(
        vkCreateDescriptorPool( GetAdaterInternal()->GetAdapterHandle(), &poolInfo, NULL, &descriptorPool ) );

    return descriptorPool;
}

// ---------------------------------------------------------------------------------------------------------------------
VkDescriptorSet EditorPipeline::CreateDescriptorSetImpl()
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
VkPipelineLayout EditorPipeline::CreatePipelineLayoutImpl()
{
    VkPipelineLayout      pipelineLayout;
    VkDescriptorSetLayout descLayout = GetDescriptorLayoutInternal();

    VkPushConstantRange pushConstantRange = {
        .stageFlags = VK_SHADER_STAGE_COMPUTE_BIT,
        .offset     = 0,
        .size       = sizeof( EditorPushConstants ),
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
VkPipeline EditorPipeline::CreatePipelineImpl()
{
    const VkDevice device   = GetAdaterInternal()->GetAdapterHandle();
    VkPipeline     pipeline = VK_NULL_HANDLE;
    m_ShaderModule =
        Shaders::LoadShader( ::B33::App::AppResources::Get().GetExecutablePathA() + "/Assets/Shaders/Editor.spv",
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
