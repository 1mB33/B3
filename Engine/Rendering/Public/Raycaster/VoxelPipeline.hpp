#ifndef B33_PIPELINE_H
#define B33_PIPELINE_H

#include "Vulkan/IPipeline.hpp"
#include "Vulkan/Buffers/GPUStreamBuffer.hpp"
#include "Vulkan/Memory/Memory.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Raycaster/PushConstants.hpp"
#include "Raycaster/VoxelGrid.hpp"

namespace B33::Rendering
{

class VoxelPipeline : public IPipeline<VoxelPipeline>
{
    using Vec  = ::B33::Math::Vec3;
    using iVec = ::B33::Math::iVec3;

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
      , m_pVoxelGrid( nullptr )
      , m_ShaderModule( VK_NULL_HANDLE )
    {
        B33_LOG( Core::Debug::Info, L"Creating a pipeline!" );
    }

    __B33_API ~VoxelPipeline();

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

    __B33_API void CreatePipelineResourcesImpl( ::std::shared_ptr<::B33::Rendering::CubeWorld> pWorld );

    __B33_API ::VkDescriptorSet CreateDescriptorSetImpl();

    __B33_API ::VkPipelineLayout CreatePipelineLayoutImpl();

    __B33_API ::VkPipeline CreatePipelineImpl();

    __B33_API ::VkDescriptorSetLayout CreateDescriptorLayoutImpl();

    __B33_API ::VkDescriptorPool CreateDescriptorPoolImpl();

  private:
    UploadDescriptor GetUniformUploadDescriptor( const ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> &outBuffer,
                                                 const EShaderResource                                      &sr );

    void LoadImage( VkImage image );

  private:
    ::B33::Rendering::VoxelPushConstants m_Vpc = {};

    // Shader uniforms
    ::std::shared_ptr<::B33::Rendering::IWorldGrid>      m_pVoxelGrid           = nullptr;
    ::std::shared_ptr<::B33::Rendering::GPUBuffer>       m_VoxelBuffer          = nullptr;
    ::std::shared_ptr<::B33::Rendering::GPUBuffer>       m_PositionsBuffer      = nullptr;
    ::std::shared_ptr<::B33::Rendering::GPUBuffer>       m_RotationsBuffer      = nullptr;
    ::std::shared_ptr<::B33::Rendering::GPUBuffer>       m_HalfSizesBuffer      = nullptr;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> m_StageVoxelBuffer     = nullptr;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> m_StagePositonsBuffer  = nullptr;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> m_StageRotationsBuffer = nullptr;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> m_StageHalfSizesBuffer = nullptr;

    ::uint32_t m_uStorageBuffersFlags = 0;

    ::VkShaderModule m_ShaderModule = VK_NULL_HANDLE;

    ::VkImageView m_ImageView = VK_NULL_HANDLE;
};

} // namespace B33::Rendering
#endif // !B33_PIPELINE_H
