#if !defined( B33_VOXEL_HPP )
#    define B33_VOXEL_HPP

#    include <B33CoreMinimal.h>

namespace B33::Rendering
{

struct alignas( 16 ) Voxel
{
    constexpr const static u32 MaxPerInstance = 26;
    constexpr const static u32 FullSolid      = -1;

    u32 Type;
    u32 Color;
    u32 Id[ MaxPerInstance ];
};

} // namespace B33::Rendering
#endif // !B33_VOXEL_HPP
