#if !defined( B33_PIPELINE_SPRITES_HPP )
#    define B33_PIPELINE_SPRITES_HPP

#    include <B33Math.hpp>

#    include "Vulkan/Buffers/GPUBuffer.hpp"
#    include "Vulkan/Buffers/ImgBuffer.hpp"
#    include "Vulkan/IPipeline.hpp"
#    include "Vulkan/Buffers/GPUStreamBuffer.hpp"

namespace B33::Rendering
{

struct alignas( 16 ) SpritesPushConstants : IPushConstants
{
    using Vec2  = ::B33::Math::Vec2;
    using Mat44 = ::B33::Math::Mat44;

    Vec2  ScreenDim;
    Mat44 Proj;
    Mat44 Cam;
};

class SpritesPipeline : public IPipeline<SpritesPipeline>
{
    using Vec3  = ::B33::Math::Vec3;
    using iVec3 = ::B33::Math::iVec3;

    template <typename T>
    constexpr decltype( auto ) MakeShared()
    {
        return ::std::make_shared<T>();
    }

    template <typename T, typename U>
    constexpr decltype( auto ) MakeShared( U &&arg )
    {
        return ::std::make_shared<T>( Forward<U>( arg ) );
    }

    template <typename T>
    using SharedPtr = ::std::shared_ptr<T>;
    template <typename T>
    using Vector = ::std::vector<T>;

    enum EShaderResource
    {
        SpriteInstances = 1,
    };

  public:
    struct alignas( 16 ) SpriteData
    {
        Vec3 Pos;
        Vec3 Color;
        Vec3 Scale;
    };

    constexpr static u32 MaxSprites = 1024;

  public:
    SpritesPipeline()
      : IPipeline( VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                   VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                   VK_PIPELINE_BIND_POINT_GRAPHICS )
      , m_pQuadBuffer( MakeShared<GPUBuffer>() )
      , m_pStageQuadBuffer( MakeShared<GPUStreamBuffer>() )
      , m_PerFrameResources()
      , m_uCurFrame( 0 )
      , m_bPendingUpload( false )
      , m_bPendingMeshUpload( true )
      , m_bPendingTextureUpload( true )
      , m_uLastUploadedGeneration( 0 )
      , m_SpriteData( MaxSprites )
      , m_VertexShader( VK_NULL_HANDLE )
    {
        B33_LOG( Core::Debug::Info, L"Creating a pipeline!" );
    }

    __B33_API ~SpritesPipeline() noexcept;

  public:
    __B33_API virtual void Update() override final;

    __B33_API virtual void RecordCommands( VkCommandBuffer        &cmdBuffer,
                                           VkPipelineStageFlagBits lastStage,
                                           VkImageLayout           lastLayout ) override final;

    __B33_API virtual void Reset() override final;

    void UpdateSprite( usize i, SpriteData sd )
    {
        m_bPendingUpload  = true;
        m_SpriteData[ i ] = sd;
    }

  public:
    usize GetPushConstantsByteSizeImpl()
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

  private:
    UploadDescriptor GetUniformUploadDescriptor( const SharedPtr<GPUStreamBuffer> &outBuffer,
                                                 const EShaderResource            &sr );

  private:
    SpritesPushConstants       m_Vpc = {};
    ImgBuffer                  m_pTexture;
    SharedPtr<GPUBuffer>       m_pStageTexture;
    SharedPtr<GPUBuffer>       m_pQuadBuffer;
    SharedPtr<GPUStreamBuffer> m_pStageQuadBuffer;

    struct PerFrame
    {
        bool                       bInit;
        u64                        uLastUploadedGeneration;
        SharedPtr<GPUBuffer>       SpriteInstances;
        SharedPtr<GPUStreamBuffer> StageSpriteInstances;
        ImgBuffer                  DepthImg;
        ::VkDescriptorSet          DescSet = VK_NULL_HANDLE;
    };

    Vector<PerFrame> m_PerFrameResources       = {};
    u32              m_uCurFrame               = -1;
    bool             m_bPendingUpload          = false;
    bool             m_bPendingMeshUpload      = false;
    bool             m_bPendingTextureUpload   = false;
    u64              m_uLastUploadedGeneration = -1;

    Vector<SpriteData> m_SpriteData = {};


    ::VkShaderModule m_VertexShader = VK_NULL_HANDLE;
    ::VkShaderModule m_FragShader   = VK_NULL_HANDLE;
};

} // namespace B33::Rendering
#endif // !B33_PIPELINE_SPRITES_HPP
