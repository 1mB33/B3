#include "B33Rendering.h"

#include "Vulkan/Utility.hpp"

#include "Vulkan/ErrorHandling.hpp"

namespace B33::Rendering
{

using namespace std;

VkShaderModule Shaders::LoadShader( const String &strPath, const AdapterWrapper *pAdapter )
{
    vector<char>   vBuffer;
    usize          uFileSize;
    VkShaderModule shaderModule;

    ifstream file( strPath, ios::ate | ios::binary );

    if ( !file.is_open() )
    {
        B33_LOG( Core::Debug::Error, L"%s", strPath.c_str() );
        throw B33_EXCEPT( "Failed to open shader file!" );
    }

    uFileSize = static_cast<usize>( file.tellg() );
    vBuffer.resize( uFileSize );

    file.seekg( 0 );
    file.read( &vBuffer[ 0 ], uFileSize );

    file.close();

    VkShaderModuleCreateInfo shaderCreateInfo = {};
    shaderCreateInfo.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderCreateInfo.codeSize                 = vBuffer.size(); // Size is in bytes, so it's okay
    shaderCreateInfo.pCode                    = reinterpret_cast<const u32 *>( &vBuffer[ 0 ] );

    THROW_IF_FAILED( vkCreateShaderModule( pAdapter->GetAdapterHandle(), &shaderCreateInfo, NULL, &shaderModule ) );

    return shaderModule;
}

}; // namespace B33::Rendering
