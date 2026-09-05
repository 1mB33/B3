#if !defined( B33_EDITOR_PUSH_CONSTANTS_HPP )
#    define B33_EDITOR_PUSH_CONSTANTS_HPP

#    include "Vulkan/IPushConstants.hpp"

namespace B33::Rendering
{

struct alignas( 16 ) EditorPushConstants : IPushConstants
{
    using Vec  = ::B33::Math::Vec3;
    using iVec = ::B33::Math::iVec3;

    Vec   CameraPos;
    iVec  GridSize;
    Vec   CameraLookDir;
    Vec   CameraRight;
    Vec   CameraUp;
    float fFov;
    u32   uMode;
    u32   _Padding2;
    u32   _Padding3;
};

} // namespace B33::Rendering
#endif // !B33_EDITOR_PUSH_CONSTANTS_HPP
