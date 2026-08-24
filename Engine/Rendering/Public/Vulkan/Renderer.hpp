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
    static inline uint64_t TIMEOUT_MAX = 1000000000;

    using FramesArray = ::std::array<::B33::Rendering::Frame, ::B33::Rendering::Frame::MAX_FRAMES_IN_FLIGHT>;
    template <typename T>
    using SharedPtr = ::std::shared_ptr<T>;
    template <typename T>
    using UniquePtr = ::std::unique_ptr<T>;
    template <typename T, typename U>
    using UnorderedMap = ::std::unordered_map<T, U>;
    template <typename T>
    using Vector          = ::std::vector<T>;
    using size_t          = ::size_t;
    using Instance        = ::B33::Rendering::Instance;
    using HardwareWrapper = ::B33::Rendering::HardwareWrapper;
    using AdapterWrapper  = ::B33::Rendering::AdapterWrapper;
    using Swapchain       = ::B33::Rendering::Swapchain;

  public:
    __B33_API Renderer();

    __B33_API ~Renderer();

    // Getters // -----------------------------------------------------------------------------------------------------
  public:
    template <class PIPE_LINE>
    PIPE_LINE *GetPipeline()
    {
        return reinterpret_cast<PIPE_LINE *>( m_PipelineMap[ PIPE_LINE::GetGlobalIndex() ] );
    }

    Swapchain *GetSwapchain()
    {
        return m_pSwapChain.get();
    }

    // Methods // -----------------------------------------------------------------------------------------------------
  public:
    template <class HARDWARE, class ADAPTER>
    void Initialize( SharedPtr<::WindowDesc> wd )
    {
        using ::std::make_shared;

        B33_LOG( Core::Debug::Info, L"Initializing renderer!" );

        m_pInstance = make_shared<Instance>();

        m_pHardware = make_shared<HardwareWrapper>();
        m_pHardware->Initialize( m_pInstance, HARDWARE() );

        m_pDeviceAdapter = make_shared<AdapterWrapper>();
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

        pipeline->Initialize( m_pDeviceAdapter, m_pMemory, m_pSwapChain.get(), *pipeline );
        pipeline->CreatePipelineResources( args... );

        m_PipelineMap[ PIPE_LINE::GetGlobalIndex() ] = pipeline;
        m_vPipelines.push_back( pipeline );
    }

    // Internal // ----------------------------------------------------------------------------------------------------
  private:
    __B33_API void InitializeInternal( SharedPtr<::WindowDesc> wd );

    ::VkCommandPool CreateCommandPool( SharedPtr<const AdapterWrapper> da, ::uint32_t uQueueFamily );

    ::VkCommandBuffer CreateCommandBuffer( SharedPtr<const AdapterWrapper> da, ::VkCommandPool cmdPool );

    FramesArray CreateFrameResources( const SharedPtr<const AdapterWrapper> &da,
                                      const SharedPtr<Memory>               &memory,
                                      ::VkCommandPool                        cmdPool,
                                      size_t                                 uFrames );

    void CreateRenederSyncResources( const SharedPtr<const AdapterWrapper> &da,
                                     const Swapchain                       *sc,
                                     Vector<VkSemaphore>                   &out );

    void RecordCommands( ::VkCommandBuffer &cmdBuff );

    void DestroyFrameResources();

    void RecreateSwapChain();

  private:
    SharedPtr<::WindowDesc>    m_pWindowDesc    = nullptr;
    SharedPtr<Instance>        m_pInstance      = nullptr;
    SharedPtr<HardwareWrapper> m_pHardware      = nullptr;
    SharedPtr<AdapterWrapper>  m_pDeviceAdapter = nullptr;
    UniquePtr<Swapchain>       m_pSwapChain     = nullptr;
    SharedPtr<Memory>          m_pMemory        = nullptr;

    UnorderedMap<::B33::Core::UnknownIndex, PipelineWrapper *> m_PipelineMap = {};
    Vector<PipelineWrapper *>                                  m_vPipelines  = {};

    ::VkCommandPool m_CommandPool = VK_NULL_HANDLE;

    size_t                 m_uCurrentFrame;
    UniquePtr<FramesArray> m_vFrames = nullptr;
    Vector<VkSemaphore>    m_vRenderFinished;
};

} // namespace B33::Rendering
#endif // !B33_RENDERER_H
