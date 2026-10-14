#include "Math.hlsl"

// --------------------------------------------------------------------------------------------------------------------
bool IntersectRayAABB( in const float3 ro,
                       in const float3 rd,
                       in const float3 halfSize,
                       out float       tMin,
                       out float       tMax )
{
    const float3 invDir   = 1.0 / rd;
    const float3 t0s      = ( -halfSize - ro ) * invDir;
    const float3 t1s      = ( halfSize - ro ) * invDir;
    const float3 tsmaller = min( t0s, t1s );
    const float3 tbigger  = max( t0s, t1s );

    tMin = max( max( tsmaller.x, tsmaller.y ), tsmaller.z );
    tMax = min( min( tbigger.x, tbigger.y ), tbigger.z );

    return tMax >= max( tMin, 0.0 );
}

// --------------------------------------------------------------------------------------------------------------------
float3 CubeNormal( in const float3 p, in const float3 halfSize )
{
    const float bias = 1.000001;
    return trunc( p / abs( halfSize ) * bias );
}

// --------------------------------------------------------------------------------------------------------------------
bool RayIntersectsAABB( in const float3 ro,
                        in const float3 rd,
                        in const float3 cubePos,
                        in const float3 cubeRot,
                        in const float3 cubeHalfSize,
                        out float       hitMin,
                        out float       hitMax,
                        out float3      normal )
{
    const float3x3 cubeRotMat = RotationMatrix( cubeRot );
    const float3   lro        = mul( cubeRotMat, ro - cubePos );
    const float3   lrd        = mul( cubeRotMat, rd );

    if ( !IntersectRayAABB( lro, lrd, cubeHalfSize, hitMin, hitMax ) )
    {
        return false;
    }

    normal = normalize( mul( CubeNormal( lro + lrd * hitMin, cubeHalfSize ), cubeRotMat ) );

    return true;
}
