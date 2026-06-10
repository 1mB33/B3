#include "ExportImport.h"
#if !defined( B33_EDITOR_PIPELINE_HPP )
#    define B33_EDITOR_PIPELINE_HPP

#    include "Vulkan/IPipeline.hpp"
#    include "Editor/PushConstants.hpp"

namespace B33::Rendering
{

class EditorPipeline : public IPipeline<EditorPipeline>
{
    using Vec  = ::B33::Math::Vec3;
    using iVec = ::B33::Math::iVec3;

  public:
    __B33_API EditorPipeline();

    __B33_API ~EditorPipeline();

    // Methods // -----------------------------------------------------------------------------------------------------
  public:
    __B33_API virtual void Update() override final;

    __B33_API virtual void RecordCommands( VkCommandBuffer        &cmdBuffer,
                                           VkPipelineStageFlagBits lastStage,
                                           VkImageLayout           lastLayout ) override final;

    __B33_API virtual void Reset() override final;

    // Getters // -----------------------------------------------------------------------------------------------------
  public:
    ::size_t GetPushConstantsByteSizeImpl()
    {
        return sizeof( m_Vpc );
    }

    IPushConstants *GetPushConstantsImpl()
    {
        return &m_Vpc;
    }

    // Interface Implementation // ------------------------------------------------------------------------------------
  public:
    __B33_API void CreatePipelineResourcesImpl();

    __B33_API ::VkDescriptorSet CreateDescriptorSetImpl();

    __B33_API ::VkPipelineLayout CreatePipelineLayoutImpl();

    __B33_API ::VkPipeline CreatePipelineImpl();

    __B33_API ::VkDescriptorSetLayout CreateDescriptorLayoutImpl();

    __B33_API ::VkDescriptorPool CreateDescriptorPoolImpl();

  private:
    void LoadImage( VkImage image );

  private:
    ::B33::Rendering::EditorPushConstants m_Vpc          = {};
    ::VkShaderModule                      m_ShaderModule = VK_NULL_HANDLE;
    ::VkImageView                         m_ImageView    = VK_NULL_HANDLE;
};

} // namespace B33::Rendering

#endif // !B33_EDITOR_PIPELINE_HPP
