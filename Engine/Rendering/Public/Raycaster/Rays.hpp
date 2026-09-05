#if !defined( B33_RAYS_HPP )
#    define B33_RAYS_HPP

#    include "Raycaster/VoxelGrid.hpp"

namespace B33::Rendering
{

struct HitResult
{
    using Vec  = ::B33::Math::Vec3;
    using iVec = ::B33::Math::iVec3;

    bool  bHit;
    iVec  iHitCoords;
    usize uHitIndex;
    float fDistance;
    Vec   Normal;
};

__B33_API HitResult MarchTheRay( const IWorldGrid        *worldGrid,
                                 const ::B33::Math::Vec3 &rayOrigin,
                                 const ::B33::Math::Vec3 &rayDestination,
                                 usize                    maxSteps );

} // namespace B33::Rendering
#endif //! B33_RAYS_HPP
