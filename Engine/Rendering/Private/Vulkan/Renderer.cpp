#include "B33Rendering.hpp"

#include "Vulkan/Renderer.hpp"
#include "Vulkan/Memory/Memory.hpp"
#include "Vulkan/ErrorHandling.hpp"
#include "Vulkan/FrameResources.hpp"
#include "Window/WindowListener.hpp"

namespace B33::Rendering
{

using namespace ::std;
using namespace ::B33::Math;

// Constructors // ----------------------------------------------------------------------------------------------------
Renderer::Renderer()
  : App::WindowListener()
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
  , m_vRenderFinished()
{
}

// --------------------------------------------------------------------------------------------------------------------
Renderer::~Renderer()
{
    Destroy();
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::InitializeInternal()
{
    m_pMemory = make_shared<Memory>( m_pHardware, m_pDeviceAdapter );

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
    lock_guard lg( GetWindowDesc()->mUpdated );

    if ( GetWindowDesc()->Data.LastEvent & EAbWindowEvents::ChangedBehavior )
    {
        GetWindowDesc()->Data.LastEvent &= ~EAbWindowEvents::ChangedBehavior;
        B33_WARNING( L"On update, the window just changed behavior, skipping a frame" );
        RecreateSwapChain();
        return;
    }

    uint32_t       uImageIndex;
    const VkDevice device     = m_pDeviceAdapter->GetAdapterHandle();
    Frame         &frame      = ( *m_vFrames.get() )[ m_uCurrentFrame ];
    const auto     swapchains = m_pSwapChain->GetSwapChainHandle();
    VkResult       result;

    THROW_IF_FAILED( vkWaitForFences( device, 1, &frame.InFlightFence, VK_TRUE, TIMEOUT_MAX ) );
    result = vkAcquireNextImageKHR( device,
                                    m_pSwapChain->GetSwapChainHandle(),
                                    TIMEOUT_MAX,
                                    frame.ImageAvailable,
                                    VK_NULL_HANDLE,
                                    &uImageIndex );

    if ( result != VK_SUCCESS )
    {
        if ( result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR )
        {
            B33_WARNING( L"On update, after vkAcquireNextImageKHR got %d", result );
            RecreateSwapChain();
            return;
        }

        if ( result == VK_TIMEOUT )
        {
            return;
        }

        B33_ERROR( L"Coulnd Acquire next image %d", result );
        throw B33_EXCEPT( "Coulnd Acquire next image" );
    }

    THROW_IF_FAILED( vkResetFences( device, 1, &frame.InFlightFence ) );

    m_pSwapChain->SetCurrentImage( uImageIndex );
    const auto imagesIndices = m_pSwapChain->GetCurrentImageIndex();

    RecordCommands( frame.CommandBuffer );

    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_ALL_COMMANDS_BIT };

    VkSubmitInfo submitInfo         = {};
    submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount   = 1;
    submitInfo.pWaitSemaphores      = &frame.ImageAvailable;
    submitInfo.pWaitDstStageMask    = waitStages;
    submitInfo.commandBufferCount   = 1;
    submitInfo.pCommandBuffers      = &frame.CommandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = &m_vRenderFinished[ uImageIndex ];

    THROW_IF_FAILED( vkQueueSubmit( m_pDeviceAdapter->GetQueueHandle(), 1, &submitInfo, frame.InFlightFence ) );

    VkPresentInfoKHR presentInfo   = {};
    presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = &m_vRenderFinished[ uImageIndex ];
    presentInfo.swapchainCount     = 1;
    presentInfo.pSwapchains        = &swapchains;
    presentInfo.pImageIndices      = &imagesIndices;

    result = vkQueuePresentKHR( m_pDeviceAdapter->GetQueueHandle(), &presentInfo );
    if ( result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR )
    {
        B33_WARNING( L"On render, after present got %d", result );
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
    else
        return;

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

    THROW_IF_FAILED( vkEndCommandBuffer( cmdBuff ) );
}

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
            vkWaitForFences( m_pDeviceAdapter->GetAdapterHandle(), 1, &frames[ i ].InFlightFence, VK_TRUE, 100000 );
        }
        B33_TRACE( L"All fences are done" );

        for ( auto &sem : m_vRenderFinished )
        {
            vkDestroySemaphore( m_pDeviceAdapter->GetAdapterHandle(), sem, nullptr );
        }
        for ( size_t i = 0; i < frames.size(); ++i )
        {
            B33_TRACE( L"Destroying frame %d", i );
            vkDestroySemaphore( m_pDeviceAdapter->GetAdapterHandle(), frames[ i ].ImageAvailable, nullptr );
            vkDestroyFence( m_pDeviceAdapter->GetAdapterHandle(), frames[ i ].InFlightFence, nullptr );
            vkFreeCommandBuffers( m_pDeviceAdapter->GetAdapterHandle(), m_CommandPool, 1, &frames[ i ].CommandBuffer );
        }
    }

    m_vRenderFinished.clear();
    B33_TRACE( L"All frames are destroyed" );
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::RecreateSwapChain()
{
    B33_INFO( L"Recreating swapchain" );
    if ( GetWindowDesc()->Data.bIsAlive == false )
    {
        B33_ERROR( L"RecreateSwapChain with dead window!!!" );
        return;
    }

    // Sync with GPU
    if ( m_pDeviceAdapter != nullptr )
        vkDeviceWaitIdle( m_pDeviceAdapter->GetAdapterHandle() );

    DestroyFrameResources();
    m_pSwapChain = nullptr;


    m_pSwapChain = make_unique<Swapchain>( m_pInstance,
                                           static_pointer_cast<HardwareWrapper>( m_pHardware ),
                                           static_pointer_cast<AdapterWrapper>( m_pDeviceAdapter ),
                                           GetWindowDesc() );

    for ( auto &pipeline : m_PipelineMap )
        pipeline.second->SetNewSwapChain( m_pSwapChain.get() );

    CreateRenederSyncResources( m_pDeviceAdapter, m_pSwapChain.get(), m_vRenderFinished );

    m_vFrames = make_unique<FramesArray>(
        CreateFrameResources( m_pDeviceAdapter, m_pMemory, m_CommandPool, Frame::MAX_FRAMES_IN_FLIGHT ) );
    m_uCurrentFrame = 0;


    B33_TRACE( L"Swapchain recreated" );
}

// --------------------------------------------------------------------------------------------------------------------
void Renderer::CreateRenederSyncResources( const ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> &da,
                                           const ::B33::Rendering::Swapchain                               *sc,
                                           ::std::vector<VkSemaphore>                                      &out )
{
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType                 = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    out.resize( sc->GetImageCount() );
    for ( auto &sem : out )
    {
        if ( vkCreateSemaphore( da->GetAdapterHandle(), &semaphoreInfo, NULL, &sem ) != VK_SUCCESS )
        {
            throw B33_EXCEPT( "Failed to create frame resources!" );
        }
    }

    B33_TRACE( L"Created sync resources for rendering" );
}

} // namespace B33::Rendering
