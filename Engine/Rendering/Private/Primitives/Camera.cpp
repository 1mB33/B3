
#include "B33Core.h"
#include "Operations.hpp"
#include "Primitives/Camera.hpp"

namespace B33::Rendering
{

using namespace B33::Math;
using namespace B33::Rendering;

Mat44 Camera::GenerateLookAt()
{
    const Vec3 up( 0.f, 1.f, 0.f );
    const Vec3 rot = GetRotation();
    const Vec3 pos = GetPosition();
    Vec3       front( 0.f, 0.f, 1.f );
    Mat44      result;

    front = RotateX( front, rot.x );
    front = RotateY( front, rot.y );
    front = RotateZ( front, rot.z );
    front = Normalize( front );

    Vec3 rigthDir = Normalize( Cross( up, front ) );
    Vec3 upDir    = Cross( front, rigthDir );

    result[ 0 ] = rigthDir.x;
    result[ 1 ] = rigthDir.y;
    result[ 2 ] = rigthDir.z;
    result[ 3 ] = -Dot( pos, rigthDir );

    result[ 4 ] = upDir.x;
    result[ 5 ] = upDir.y;
    result[ 6 ] = upDir.z;
    result[ 7 ] = -Dot( pos, upDir );

    result[ 8 ]  = front.x;
    result[ 9 ]  = front.y;
    result[ 10 ] = front.z;
    result[ 11 ] = -Dot( pos, front );

    result[ 12 ] = 0.f;
    result[ 13 ] = 0.f;
    result[ 14 ] = 0.f;
    result[ 15 ] = 1.f;

    return result;
}
} // namespace B33::Rendering
