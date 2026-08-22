#ifndef B33_RENDERER_H
#define B33_RENDERER_H

#include "Unknown.hpp"
#include "Vulkan/FrameResources.hpp"
#include "Vulkan/Instance.hpp"
#include "Vulkan/WrapperAdapter.hpp"
#include "Vulkan/WrapperHardware.hpp"
#include "Vulkan/Memory/Memory.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/WrapperPipeline.hpp"

namespace B33::Rendering
{

class Renderer
{
    using FramesArray = ::std::array<::B33::Rendering::Frame, ::B33::Rendering::Frame::MAX_FRAMES_IN_FLIGHT>;

  public:
    __B33_API Renderer();

    __B33_API ~Renderer();

    // Getters // -----------------------------------------------------------------------------------------------------
  public:
    template <class PIPE_LINE>
    PIPE_LINE *GetPipeline()
    {
        return reinterpret_cast<PIPE_LINE*>(m_PipelineMap[ PIPE_LINE::GetGlobalIndex() ]);
    }

    ::B33::Rendering::Swapchain *GetSwapchain()
    {
        return m_pSwapChain.get();
    }

    // Methods // -----------------------------------------------------------------------------------------------------
  public:
    template <class HARDWARE, class ADAPTER>
    void Initialize( ::std::shared_ptr<::WindowDesc> wd )
    {
        B33_LOG( Core::Debug::Info, L"Initializing renderer!" );

        m_pInstance = ::std::make_shared<Instance>();

        m_pHardware = ::std::make_shared<HardwareWrapper>();
        m_pHardware->Initialize( m_pInstance, HARDWARE() );

        m_pDeviceAdapter = ::std::make_shared<AdapterWrapper>();
        m_pDeviceAdapter->Initialize( m_pHardware, ADAPTER() );

        this->InitializeInternal( wd );
    }

    __B33_API void Update( const float fDelta );

    __B33_API void Render();

    __B33_API void Destroy();

    /**
     * @brief Pushes new pipeline stage on to rendering stack
     *
     * @tparam PIPE_LINE Class derived from IPipeline class
     * @tparam RESOURCES_ARGS Types of arguments passed to the pipeline CreatePipelineResources method implementation
     * @param args Arguments passed to the pipeline CreatePipelineResources method implementation
     */
    template <class PIPE_LINE, class... RESOURCES_ARGS>
    void PushPipeline( RESOURCES_ARGS... args )
    {
        auto pipeline = new PIPE_LINE();

        pipeline->Initialize( m_pDeviceAdapter, m_pMemory, m_pSwapChain, *pipeline );
        pipeline->CreatePipelineResources( args... );

        m_PipelineMap[ PIPE_LINE::GetGlobalIndex() ] = pipeline;
        m_vPipelines.push_back( pipeline );
    }

    // Internal // ----------------------------------------------------------------------------------------------------
  private:
    __B33_API void InitializeInternal( ::std::shared_ptr<::WindowDesc> wd );

    ::VkCommandPool CreateCommandPool( ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> da,
                                       ::uint32_t                                                uQueueFamily );

    ::VkCommandBuffer CreateCommandBuffer( ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> da,
                                           ::VkCommandPool                                           cmdPool );

    FramesArray CreateFrameResources( const ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> &da,
                                      const ::std::shared_ptr<::B33::Rendering::Memory>               &memory,
                                      ::VkCommandPool                                                  cmdPool,
                                      ::size_t                                                         uFrames );

    void RecordCommands( ::VkCommandBuffer &cmdBuff );

    void DestroyFrameResources();

    void RecreateSwapChain();

  private:
    ::std::shared_ptr<::WindowDesc>                      m_pWindowDesc    = nullptr;
    ::std::shared_ptr<::B33::Rendering::Instance>        m_pInstance      = nullptr;
    ::std::shared_ptr<::B33::Rendering::HardwareWrapper> m_pHardware      = nullptr;
    ::std::shared_ptr<::B33::Rendering::AdapterWrapper>  m_pDeviceAdapter = nullptr;
    ::std::shared_ptr<::B33::Rendering::Swapchain>       m_pSwapChain     = nullptr;
    ::std::shared_ptr<::B33::Rendering::Memory>          m_pMemory        = nullptr;

    ::std::unordered_map<::B33::Core::UnknownIndex, ::B33::Rendering::PipelineWrapper *> m_PipelineMap = {};
    ::std::vector<::B33::Rendering::PipelineWrapper *>                                   m_vPipelines  = {};

    ::VkCommandPool m_CommandPool = VK_NULL_HANDLE;

    ::size_t                       m_uCurrentFrame;
    ::std::unique_ptr<FramesArray> m_vFrames = nullptr;
};

} // namespace B33::Rendering
#endif // !B33_RENDERER_H
