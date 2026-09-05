#if !defined( B33_PIPELINE_HPP )
#    define B33_PIPELINE_HPP

#    include "Vulkan/IPipeline.hpp"
#    include "Vulkan/Buffers/GPUStreamBuffer.hpp"
#    include "Raycaster/PushConstants.hpp"
#    include "Raycaster/VoxelGrid.hpp"
#    include "vulkan/vulkan_core.h"

namespace B33::Rendering
{

class VoxelPipeline : public IPipeline<VoxelPipeline>
{
    using Vec  = ::B33::Math::Vec3;
    using iVec = ::B33::Math::iVec3;
    template <typename T>
    using SharedPtr = ::std::shared_ptr<T>;
    template <typename T>
    using Vector = ::std::vector<T>;

  private:
    enum EShaderResource
    {
        VoxelGrid       = 1,
        ObjectPositions = VoxelGrid + 1,
        ObjectRotations = ObjectPositions + 1,
        ObjectHalfSizes = ObjectRotations + 1,
    };

  public:
    VoxelPipeline()
      : IPipeline( VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_IMAGE_LAYOUT_GENERAL, VK_PIPELINE_BIND_POINT_COMPUTE )
      , m_PerFrameResources()
      , m_ShaderModule( VK_NULL_HANDLE )
    {
        B33_LOG( Core::Debug::Info, L"Creating a pipeline!" );
    }

    __B33_API ~VoxelPipeline() noexcept;

  public:
    __B33_API virtual void Update() override final;

    __B33_API virtual void RecordCommands( VkCommandBuffer        &cmdBuffer,
                                           VkPipelineStageFlagBits lastStage,
                                           VkImageLayout           lastLayout ) override final;

    __B33_API virtual void Reset() override final;

  public:
    usize GetPushConstantsByteSizeImpl()
    {
        return sizeof( m_Vpc );
    }

    IPushConstants *GetPushConstantsImpl()
    {
        return &m_Vpc;
    }

    __B33_API void CreatePipelineResourcesImpl( SharedPtr<CubeWorld> pWorld );

    __B33_API ::VkDescriptorSet CreateDescriptorSet();

    __B33_API ::VkPipelineLayout CreatePipelineLayoutImpl();

    __B33_API ::VkPipeline CreatePipelineImpl();

    __B33_API ::VkDescriptorSetLayout CreateDescriptorLayoutImpl();

    __B33_API ::VkDescriptorPool CreateDescriptorPoolImpl();

  private:
    UploadDescriptor GetUniformUploadDescriptor( const SharedPtr<GPUStreamBuffer> &outBuffer,
                                                 const EShaderResource            &sr );

    void LoadImage( VkImageView image );

  private:
    VoxelPushConstants m_Vpc = {};

    // Shader uniforms
    SharedPtr<IWorldGrid> m_pVoxelGrid = nullptr;

    struct PerFrame
    {
        SharedPtr<GPUBuffer>       pVoxelBuffer             = nullptr;
        SharedPtr<GPUBuffer>       pPositionsBuffer         = nullptr;
        SharedPtr<GPUBuffer>       pRotationsBuffer         = nullptr;
        SharedPtr<GPUBuffer>       pHalfSizesBuffer         = nullptr;
        SharedPtr<GPUStreamBuffer> pStageVoxelBuffer        = nullptr;
        SharedPtr<GPUStreamBuffer> pStagePositonsBuffer     = nullptr;
        SharedPtr<GPUStreamBuffer> pStageRotationsBuffer    = nullptr;
        SharedPtr<GPUStreamBuffer> pStageHalfSizesBuffer    = nullptr;
        u32                        uStorageBuffersFlags     = 0;
        u32                        uLastStorageBuffersFlags = 0;
        ::VkDescriptorSet          DescSet                  = VK_NULL_HANDLE;
    };

    Vector<PerFrame> m_PerFrameResources = {};
    u32              m_uCurFrame         = -1;


    ::VkShaderModule m_ShaderModule = VK_NULL_HANDLE;
};

} // namespace B33::Rendering
#endif // !B33_PIPELINE_HPP
