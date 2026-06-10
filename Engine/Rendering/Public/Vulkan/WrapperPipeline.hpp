#ifndef B33_WRAPPER_PIPELINE_H
#define B33_WRAPPER_PIPELINE_H

#include "Attributes.h"
#include "Vulkan/IPushConstants.hpp"
#include "Vulkan/Memory/Memory.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/WrapperAdapter.hpp"

namespace B33::Rendering
{

class PipelineWrapper
{
    using Vec  = ::B33::Math::Vec3;
    using iVec = ::B33::Math::iVec3;

  public:
    PipelineWrapper() = delete;

    PipelineWrapper( VkPipelineStageFlagBits stage, VkImageLayout imgLayout, VkPipelineBindPoint bindPoint )
      : m_pDeviceAdapter()
      , m_pMemory()
      , m_pWindowDesc()
      , m_pSwapChain()
      , m_StageBits( stage )
      , m_ImageLayout( imgLayout )
      , m_BindPoint( bindPoint )
    {
    }

    virtual ~PipelineWrapper()
    {
        B33_LOG( Core::Debug::Info, L"Destroying pipeline" );
        if ( m_Pipeline != VK_NULL_HANDLE )
        {
            vkDestroyPipeline( GetAdaterInternal()->GetAdapterHandle(), m_Pipeline, NULL );
            m_Pipeline = VK_NULL_HANDLE;
        }
        if ( m_PipelineLayout != VK_NULL_HANDLE )
        {
            vkDestroyPipelineLayout( GetAdaterInternal()->GetAdapterHandle(), m_PipelineLayout, NULL );
            m_PipelineLayout = VK_NULL_HANDLE;
        }
        if ( m_DescriptorPool != VK_NULL_HANDLE )
        {
            vkDestroyDescriptorPool( GetAdaterInternal()->GetAdapterHandle(), m_DescriptorPool, NULL );
            m_DescriptorPool = VK_NULL_HANDLE;
        }
        if ( m_DescriptorLayout != VK_NULL_HANDLE )
        {
            vkDestroyDescriptorSetLayout( GetAdaterInternal()->GetAdapterHandle(), m_DescriptorLayout, NULL );
            m_DescriptorLayout = VK_NULL_HANDLE;
        }
    }

  public:
    PipelineWrapper( PipelineWrapper && ) noexcept = delete;
    PipelineWrapper( const PipelineWrapper & )     = delete;

    PipelineWrapper &operator=( PipelineWrapper && ) noexcept      = delete;
    PipelineWrapper &operator=( const PipelineWrapper & ) noexcept = delete;

  public:
    template <class T>
    void Initialize( ::std::weak_ptr<const ::B33::Rendering::AdapterWrapper> pDeviceAdapter,
                     ::std::weak_ptr<::B33::Rendering::Memory>               pMemory,
                     ::std::weak_ptr<const ::WindowDesc>                     pWindowDesc,
                     ::std::weak_ptr<const ::B33::Rendering::Swapchain>      pSwapChain,
                     T                                                      &pPipeline )
    {
        B33_LOG( Core::Debug::Info, L"Initializing pipeline" );

        m_pDeviceAdapter = pDeviceAdapter;
        m_pMemory        = pMemory;
        m_pWindowDesc    = pWindowDesc;
        m_pSwapChain     = pSwapChain;

        m_uPushConstantsByteSize = pPipeline.GetPushConstantsByteSize();
        m_pPushConstants         = pPipeline.GetPushConstants();
        m_DescriptorLayout       = pPipeline.CreateDescriptorLayout();
        m_DescriptorPool         = pPipeline.CreateDescriptorPool();
        m_DescriptorSet          = pPipeline.CreateDescriptorSet();
        m_PipelineLayout         = pPipeline.CreatePipelineLayout();
        m_Pipeline               = pPipeline.CreatePipeline();
    }

    virtual void Update() = 0;

    virtual void
    RecordCommands( VkCommandBuffer &cmdBuffer, VkPipelineStageFlagBits lastStage, VkImageLayout lastLayout ) = 0;

    virtual void Reset() = 0;

    void LoadPushConstants( const IPushConstants &constants, __B33_ATTRIBUTE_MIGHT_BE_UNUSED ::size_t uByteSize )
    {
        B33_ASSERT( uByteSize == m_uPushConstantsByteSize );

        memcpy( m_pPushConstants, &constants, m_uPushConstantsByteSize );
    }

  public:
    ::VkPipelineLayout GetLayoutHandle() const
    {
        return m_PipelineLayout;
    }

    ::VkPipeline GetPipelineHandle() const
    {
        return m_Pipeline;
    }

    ::VkDescriptorSet &GetDescriptorSet()
    {
        return m_DescriptorSet;
    }

    ::VkPipelineStageFlagBits GetPipelineStageFlagBits() const
    {
        return m_StageBits;
    }

    ::VkImageLayout GetImageLayout() const
    {
        return m_ImageLayout;
    }

    ::VkPipelineBindPoint GetPipelineBindPoint() const
    {
        return m_BindPoint;
    }

  public:
    void SetNewSwapChain( ::std::weak_ptr<const ::B33::Rendering::Swapchain> pSwapChain )
    {
        m_pSwapChain = pSwapChain;
    }

  protected:
    ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> GetAdaterInternal()
    {
        if ( auto result = m_pDeviceAdapter.lock() )
            return result;

        throw B33_EXCEPT( "Cannot lock resources in the PipelineWrapper" );
    }

    ::std::shared_ptr<::B33::Rendering::Memory> GetMemoryInternal() const
    {
        if ( auto result = m_pMemory.lock() )
            return result;

        throw B33_EXCEPT( "Cannot lock resources in the PipelineWrapper" );
    }

    ::std::shared_ptr<const ::WindowDesc> GetWindowDescInternal() const
    {
        if ( auto result = m_pWindowDesc.lock() )
            return result;

        throw B33_EXCEPT( "Cannot lock resources in the PipelineWrapper" );
    }

    ::std::shared_ptr<const ::B33::Rendering::Swapchain> GetSwapChainInternal() const
    {
        if ( auto result = m_pSwapChain.lock() )
            return result;

        throw B33_EXCEPT( "Cannot lock resources in the PipelineWrapper" );
    }

    ::VkDescriptorSetLayout GetDescriptorLayoutInternal()
    {
        return m_DescriptorLayout;
    }

    ::VkDescriptorPool GetDescriptorPoolInternal()
    {
        return m_DescriptorPool;
    }

  private:
    ::std::weak_ptr<const ::B33::Rendering::AdapterWrapper> m_pDeviceAdapter = {};
    ::std::weak_ptr<::B33::Rendering::Memory>               m_pMemory        = {};
    ::std::weak_ptr<const ::WindowDesc>                     m_pWindowDesc    = {};
    ::std::weak_ptr<const ::B33::Rendering::Swapchain>      m_pSwapChain     = {};

    ::size_t        m_uPushConstantsByteSize = 0;
    IPushConstants *m_pPushConstants         = nullptr;

    ::VkPipelineStageFlagBits m_StageBits   = {};
    ::VkImageLayout           m_ImageLayout = {};
    ::VkPipelineBindPoint     m_BindPoint   = {};

    ::VkDescriptorSetLayout m_DescriptorLayout = VK_NULL_HANDLE;
    ::VkDescriptorPool      m_DescriptorPool   = VK_NULL_HANDLE;
    ::VkDescriptorSet       m_DescriptorSet    = VK_NULL_HANDLE;
    ::VkPipelineLayout      m_PipelineLayout   = VK_NULL_HANDLE;
    ::VkPipeline            m_Pipeline         = VK_NULL_HANDLE;
};

} // namespace B33::Rendering
#endif // !B33_WRAPPER_PIPELINE_H
