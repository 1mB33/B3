#ifndef B33_PIPELINE_SPRITES_H
#define B33_PIPELINE_SPRITES_H

#include "Vec2.hpp"
#include "Vulkan/IPipeline.hpp"
#include "Vulkan/Buffers/GPUStreamBuffer.hpp"
#include "vulkan/vulkan_core.h"

namespace B33::Rendering
{

struct alignas( 16 ) SpritesPushConstants : IPushConstants
{
    using Vec2 = ::B33::Math::Vec2;

    Vec2 ScreenDim;
};

class SpritesPipeline : public IPipeline<SpritesPipeline>
{
    using Vec  = ::B33::Math::Vec3;
    using iVec = ::B33::Math::iVec3;

    enum EShaderResource
    {
        SpriteInstances = 1,
    };

  public:
    struct alignas( 16 ) SpriteData
    {
        ::B33::Math::Vec3 Pos;
        ::B33::Math::Vec3 Color;
        ::B33::Math::Vec3 Scale;
    };

    constexpr static uint32_t MAX_SPRITES = 1024;

  public:
    SpritesPipeline()
      : IPipeline( VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                   VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                   VK_PIPELINE_BIND_POINT_GRAPHICS )
      , m_pQuadBuffer( ::std::make_shared<GPUBuffer>() )
      , m_pStageQuadBuffer( ::std::make_shared<GPUStreamBuffer>() )
      , m_PerFrameResources()
      , m_SpriteData( MAX_SPRITES )
      , m_VertexShader( VK_NULL_HANDLE )
    {
        B33_LOG( Core::Debug::Info, L"Creating a pipeline!" );
    }

    __B33_API ~SpritesPipeline();

  public:
    __B33_API virtual void Update() override final;

    __B33_API virtual void RecordCommands( VkCommandBuffer        &cmdBuffer,
                                           VkPipelineStageFlagBits lastStage,
                                           VkImageLayout           lastLayout ) override final;

    __B33_API virtual void Reset() override final;

    ::size_t GetPushConstantsByteSizeImpl()
    {
        return sizeof( m_Vpc );
    }

    IPushConstants *GetPushConstantsImpl()
    {
        return &m_Vpc;
    }

    __B33_API void CreatePipelineResourcesImpl();

    __B33_API ::VkDescriptorSet CreateDescriptorSet();

    __B33_API ::VkPipelineLayout CreatePipelineLayoutImpl();

    __B33_API ::VkPipeline CreatePipelineImpl();

    __B33_API ::VkDescriptorSetLayout CreateDescriptorLayoutImpl();

    __B33_API ::VkDescriptorPool CreateDescriptorPoolImpl();

  public:
    void UpdateSprite( size_t i, SpriteData sd )
    {
        m_SpriteData[ i ] = sd;
    }

  private:
    UploadDescriptor GetUniformUploadDescriptor( const ::std::shared_ptr<GPUStreamBuffer> &outBuffer,
                                                 const EShaderResource                    &sr );

  private:
    ::B33::Rendering::SpritesPushConstants m_Vpc = {};

    ::std::shared_ptr<::B33::Rendering::GPUBuffer>       m_pQuadBuffer;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> m_pStageQuadBuffer;

    struct PerFrame
    {
        ::uint32_t                                           uLastUploadedGeneration;
        bool                                                 bPendingGpuCopy;
        ::std::shared_ptr<::B33::Rendering::GPUBuffer>       SpriteInstances;
        ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> StageSpriteInstances;
        VkImage                                              DepthImg;
        VkImageView                                          DepthImgView;
        ::VkDescriptorSet                                    DescSet = VK_NULL_HANDLE;
    };

    ::std::vector<PerFrame> m_PerFrameResources = {};
    ::uint32_t              m_uCurFrame         = -1;

    ::std::vector<SpriteData> m_SpriteData = {};


    ::VkShaderModule m_VertexShader = VK_NULL_HANDLE;
    ::VkShaderModule m_FragShader   = VK_NULL_HANDLE;
};

} // namespace B33::Rendering
#endif // !B33_PIPELINE_SPRITES_H
