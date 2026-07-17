#include "B33Rendering.hpp"

#include "Vulkan/Renderer.hpp"
#include "Vulkan/BasicComponentImplementations/GraphicsComputeAdapter.hpp"
#include "Vulkan/BasicComponentImplementations/MinimalHardware.hpp"
#include "Vulkan/Memory/Memory.hpp"
#include "Vulkan/ErrorHandling.hpp"
#include "Vulkan/FrameResources.hpp"
#include "Vulkan/WrapperAdapter.hpp"
#include "Vulkan/WrapperHardware.hpp"
#include <utility>

namespace B33::Rendering
{

using namespace ::std;
using namespace ::B33::Math;

// Constructors // ----------------------------------------------------------------------------------------------------
Renderer::Renderer()
  : m_pWindowDesc( nullptr )
  , m_pInstance( nullptr )
  , m_pHardware( nullptr )
  , m_pDeviceAdapter( nullptr )
  , m_pSwapChain( nullptr )
  , m_pMemory( nullptr )
  , m_PipelineMap()
  , m_vPipelines()
  , m_CommandPool( VK_NULL_HANDLE )
  , m_uCurrentFrame( 0 )
  , m_vFrames()
{
}

// --------------------------------------------------------------------------------------------------------------------
Renderer::~Renderer()
{
    Destroy();
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::Initialize( shared_ptr<WindowDesc> wd )
{
    B33_LOG( Core::Debug::Info, L"Initializing renderer!" );

    m_pInstance = make_shared<Instance>();

    m_pHardware = make_shared<HardwareWrapper>();
    m_pHardware->Initialize( m_pInstance, MinimalHardware() );

    m_pDeviceAdapter = make_shared<AdapterWrapper>();
    m_pDeviceAdapter->Initialize( m_pHardware, GraphicsComputeAdapter() );

    m_pMemory     = make_shared<Memory>( m_pHardware, m_pDeviceAdapter );
    m_pWindowDesc = wd;

    B33_TRACE( L"Initializing command pool" );
    m_CommandPool = CreateCommandPool( static_pointer_cast<AdapterWrapper>( m_pDeviceAdapter ),
                                       m_pDeviceAdapter->GetQueueFamilyIndex() );

    // Recreating swap chain also creates frame resources and initializes swap chain
    RecreateSwapChain();
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::Update( const float )
{
    for ( auto &pPipeline : m_vPipelines )
    {
        pPipeline->Update();
    }
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::Render()
{
    lock_guard lg( m_pWindowDesc->mUpdated );

    if ( m_pWindowDesc->Data.LastEvent & EAbWindowEvents::ChangedBehavior )
    {
        B33_WARNING( L"On update, the window just changed behavior, skipping a frame" );
        RecreateSwapChain();
        return;
    }

    uint32_t uImageIndex;
    VkDevice device = m_pDeviceAdapter->GetAdapterHandle();
    Frame   &frame  = ( *m_vFrames.get() )[ m_uCurrentFrame ];
    VkResult result;

    THROW_IF_FAILED( vkWaitForFences( device, 1, &frame.InFlightFence, VK_TRUE, UINT64_MAX ) );
    THROW_IF_FAILED( vkResetFences( device, 1, &frame.InFlightFence ) );

    result = vkAcquireNextImageKHR( device,
                                    m_pSwapChain->GetSwapChainHandle(),
                                    UINT64_MAX,
                                    frame.ImageAvailable,
                                    VK_NULL_HANDLE,
                                    &uImageIndex );

    if ( result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR )
    {
        B33_ERROR( L"On update, after vkAcquireNextImageKHR got %d", result );
        RecreateSwapChain();
        return;
    }

    m_pSwapChain->SetCurrentImage( uImageIndex );

    RecordCommands( frame.CommandBuffer );

    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    VkSubmitInfo submitInfo         = {};
    submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount   = 1;
    submitInfo.pWaitSemaphores      = &frame.ImageAvailable;
    submitInfo.pWaitDstStageMask    = waitStages;
    submitInfo.commandBufferCount   = 1;
    submitInfo.pCommandBuffers      = &frame.CommandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = &frame.RenderFinished;

    THROW_IF_FAILED( vkQueueSubmit( m_pDeviceAdapter->GetQueueHandle(), 1, &submitInfo, frame.InFlightFence ) );

    static auto swapchains    = m_pSwapChain->GetSwapChainHandle();
    swapchains                = m_pSwapChain->GetSwapChainHandle();
    static auto imagesIndices = m_pSwapChain->GetImageindex();
    imagesIndices             = m_pSwapChain->GetImageindex();

    VkPresentInfoKHR presentInfo   = {};
    presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = &frame.RenderFinished;
    presentInfo.swapchainCount     = 1;
    presentInfo.pSwapchains        = &swapchains;
    presentInfo.pImageIndices      = &imagesIndices;

    result = vkQueuePresentKHR( m_pDeviceAdapter->GetQueueHandle(), &presentInfo );
    if ( result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR )
    {
        B33_ERROR( L"On render, after present got %d", result );
        RecreateSwapChain();
        return;
    }

    m_uCurrentFrame = ( m_uCurrentFrame + 1 ) % Frame::MAX_FRAMES_IN_FLIGHT;
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::Destroy()
{
    B33_LOG( Core::Debug::Info, L"Destroying renderer" );

    if ( m_pDeviceAdapter != nullptr )
        vkDeviceWaitIdle( m_pDeviceAdapter->GetAdapterHandle() );

    if ( m_pDeviceAdapter != nullptr )
        DestroyFrameResources();

    if ( m_CommandPool != VK_NULL_HANDLE )
        vkDestroyCommandPool( m_pDeviceAdapter->GetAdapterHandle(), m_CommandPool, nullptr );

    for ( auto &pPipeline : m_vPipelines )
    {
        delete pPipeline;
        B33_TRACE( L"Deleted pipeline from renderer" );
    }
    m_PipelineMap.clear();
    m_vPipelines.clear();

    m_CommandPool    = VK_NULL_HANDLE;
    m_pMemory        = nullptr;
    m_pSwapChain     = nullptr;
    m_pDeviceAdapter = nullptr;
    m_pHardware      = nullptr;
    m_pInstance      = nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------
VkCommandPool Renderer::CreateCommandPool( shared_ptr<const AdapterWrapper> da, uint32_t uQueueFamily )
{
    VkCommandPool cmdPool;

    VkCommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.flags                   = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    cmdPoolInfo.queueFamilyIndex        = uQueueFamily;

    THROW_IF_FAILED( vkCreateCommandPool( da->GetAdapterHandle(), &cmdPoolInfo, NULL, &cmdPool ) );

    return cmdPool;
}

// ---------------------------------------------------------------------------------------------------------------------
VkCommandBuffer Renderer::CreateCommandBuffer( __B33_ATTRIBUTE_MIGHT_BE_UNUSED shared_ptr<const AdapterWrapper> da,
                                               VkCommandPool cmdPool )
{
    VkCommandBuffer cmdBuffer;

    VkCommandBufferAllocateInfo allocInfo = {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext              = NULL,
        .commandPool        = cmdPool,
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    THROW_IF_FAILED( vkAllocateCommandBuffers( m_pDeviceAdapter->GetAdapterHandle(), &allocInfo, &cmdBuffer ) );
    THROW_IF_FAILED( vkResetCommandBuffer( cmdBuffer, 0 ) );

    return cmdBuffer;
}

// ---------------------------------------------------------------------------------------------------------------------
Renderer::FramesArray Renderer::CreateFrameResources( const shared_ptr<const AdapterWrapper> &da,
                                                      __B33_ATTRIBUTE_MIGHT_BE_UNUSED const shared_ptr<Memory> &memory,
                                                      VkCommandPool                                             cmdPool,
                                                      __B33_ATTRIBUTE_MIGHT_BE_UNUSED size_t uFrames )
{
    VkDevice    device = da->GetAdapterHandle();
    FramesArray result;

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType                 = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType             = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags             = VK_FENCE_CREATE_SIGNALED_BIT;

    for ( size_t i = 0; i < result.size(); ++i )
    {
        if ( vkCreateSemaphore( device, &semaphoreInfo, NULL, &result[ i ].ImageAvailable ) != VK_SUCCESS ||
             vkCreateSemaphore( device, &semaphoreInfo, NULL, &result[ i ].RenderFinished ) != VK_SUCCESS ||
             vkCreateFence( device, &fenceInfo, NULL, &result[ i ].InFlightFence ) != VK_SUCCESS )
        {
            throw B33_EXCEPT( "Failed to create frame resources!" );
        }

        result[ i ].CommandBuffer = CreateCommandBuffer( da, cmdPool );
    }

    for ( auto &pPipeline : m_vPipelines )
        pPipeline->Reset();

    return result;
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::RecordCommands( VkCommandBuffer &cmdBuff )
{
    vkResetCommandBuffer( cmdBuff, 0 );

    VkCommandBufferBeginInfo beginInfo = {
        .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext            = NULL,
        .flags            = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        .pInheritanceInfo = NULL,
    };

    THROW_IF_FAILED( vkBeginCommandBuffer( cmdBuff, &beginInfo ) );

    VkPipelineStageFlagBits lastStage     = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkImageLayout           lastImgLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    for ( auto &pPipeline : m_vPipelines )
    {
        pPipeline->RecordCommands( cmdBuff, lastStage, lastImgLayout );
        lastStage     = pPipeline->GetPipelineStageFlagBits();
        lastImgLayout = pPipeline->GetImageLayout();
    }

    VkImageMemoryBarrier presentBarrier = {};
    presentBarrier.sType                = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    presentBarrier.srcAccessMask        = VK_ACCESS_SHADER_WRITE_BIT;
    presentBarrier.dstAccessMask        = 0;
    presentBarrier.oldLayout            = lastImgLayout;
    presentBarrier.newLayout            = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    presentBarrier.srcQueueFamilyIndex  = VK_QUEUE_FAMILY_IGNORED;
    presentBarrier.dstQueueFamilyIndex  = VK_QUEUE_FAMILY_IGNORED;
    presentBarrier.image                = m_pSwapChain->GetImage();
    presentBarrier.subresourceRange     = {
        .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel   = 0,
        .levelCount     = 1,
        .baseArrayLayer = 0,
        .layerCount     = 1,
    };

    vkCmdPipelineBarrier( cmdBuff,
                          lastStage,
                          VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                          0,
                          0,
                          NULL,
                          0,
                          NULL,
                          1,
                          &presentBarrier );

    THROW_IF_FAILED( vkEndCommandBuffer( cmdBuff ) );
} // namespace B33::Rendering

// --------------------------------------------------------------------------------------------------------------------
void Renderer::DestroyFrameResources()
{
    B33_LOG( Core::Debug::Info, L"Destroying frame resources" );

    if ( m_vFrames != nullptr )
    {
        auto &frames = *m_vFrames.get();

        for ( size_t i = 0; i < frames.size(); ++i )
        {
            B33_TRACE( L"Waiting for fence %d", i );
            vkWaitForFences( m_pDeviceAdapter->GetAdapterHandle(), 1, &frames[ i ].InFlightFence, VK_TRUE, UINT64_MAX );
        }
        B33_TRACE( L"All fences are done" );

        for ( size_t i = 0; i < frames.size(); ++i )
        {
            B33_TRACE( L"Destroying frame %d", i );
            vkDestroySemaphore( m_pDeviceAdapter->GetAdapterHandle(), frames[ i ].RenderFinished, nullptr );
            vkDestroySemaphore( m_pDeviceAdapter->GetAdapterHandle(), frames[ i ].ImageAvailable, nullptr );
            vkDestroyFence( m_pDeviceAdapter->GetAdapterHandle(), frames[ i ].InFlightFence, nullptr );
            vkFreeCommandBuffers( m_pDeviceAdapter->GetAdapterHandle(), m_CommandPool, 1, &frames[ i ].CommandBuffer );
        }
    }
    B33_TRACE( L"All frames are destroyed" );
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::RecreateSwapChain()
{
    B33_INFO( L"Recreating swapchain" );
    if ( m_pWindowDesc->Data.bIsAlive == false )
    {
        B33_ERROR( L"RecreateSwapChain with dead window!!!" );
        return;
    }
    if ( m_pDeviceAdapter != nullptr )
        vkDeviceWaitIdle( m_pDeviceAdapter->GetAdapterHandle() );

    DestroyFrameResources();

    m_pSwapChain = nullptr;
    m_pSwapChain = make_shared<Swapchain>( m_pInstance,
                                           static_pointer_cast<HardwareWrapper>( m_pHardware ),
                                           static_pointer_cast<AdapterWrapper>( m_pDeviceAdapter ),
                                           m_pWindowDesc );

    m_vFrames = make_unique<FramesArray>(
        CreateFrameResources( m_pDeviceAdapter, m_pMemory, m_CommandPool, Frame::MAX_FRAMES_IN_FLIGHT ) );
    m_uCurrentFrame = 0;
    B33_TRACE( L"Swapchain recreated" );

    for ( auto &pipeline : m_PipelineMap )
    {
        pipeline.second->SetNewSwapChain( m_pSwapChain );
    }
}

} // namespace B33::Rendering
