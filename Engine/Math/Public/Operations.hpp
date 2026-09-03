#if !defined(B33_OPERATIONS_H)
#define B33_OPERATIONS_H

#include "Mat44.hpp"
#include "Vec3.hpp"

namespace B33::Math
{

// ---------------------------------------------------------------------------------------------------------------------
template <class Vector>
Vector Normalize( const Vector &v )
{
    Vector result = Vector();
    float  mod    = 0.f;
    float  invMag;

    for ( size_t i = 0; i < Vector::Size; ++i )
        mod += v[ i ] * v[ i ];

    if ( mod == 0.f )
    {
        return result;
    }

    invMag = 1.f / ::std::sqrt( mod );

    for ( size_t i = 0; i < Vector::Size; ++i )
        result[ i ] = v[ i ] * invMag;

    return result;
}

// ---------------------------------------------------------------------------------------------------------------------
template <class Vector>
constexpr inline Vector Cross( const Vector &, const Vector & )
{
    static_assert( Core::TypeIsAlwaysFalse<Vector>,
                   "This size of a vector doesn't have impementation of cross product yet" );
}

// ---------------------------------------------------------------------------------------------------------------------
template <>
inline Vec3 Cross( const Vec3 &vA, const Vec3 &vB )
{
    Vec3 result;
    result.x = vA.y * vB.z - vB.y * vA.z;
    result.y = vA.z * vB.x - vB.z * vA.x;
    result.z = vA.x * vB.y - vB.x * vA.y;
    return result;
}

// ---------------------------------------------------------------------------------------------------------------------
template <class Vector>
constexpr inline float Dot( const Vector &, const Vector & )
{
    static_assert( Core::TypeIsAlwaysFalse<Vector>,
                   "This size of a vector doesn't have impementation of dot product yet" );
    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
template <>
constexpr inline float Dot( const Vec3 &vA, const Vec3 &vB )
{
    return ( vA.x * vB.x ) + ( vA.y * vB.y ) + ( vA.z * vB.z );
}

// ---------------------------------------------------------------------------------------------------------------------
template <class Vector>
constexpr inline Vector RotateX( const Vector &, float )
{
    static_assert( Core::TypeIsAlwaysFalse<Vector>,
                   "This size of a vector doesn't have impementation of rotate x yet" );
}

// ---------------------------------------------------------------------------------------------------------------------
template <>
inline Vec3 RotateX( const Vec3 &v, float angleRad )
{
    Vec3  result;
    float c = std::cos( angleRad );
    float s = std::sin( angleRad );

    result.x = v.x;
    result.y = v.y * c - v.z * s;
    result.z = v.y * s + v.z * c;

    return result;
}

// ---------------------------------------------------------------------------------------------------------------------
template <class Vector>
constexpr inline Vector RotateY( const Vector &, float )
{
    static_assert( Core::TypeIsAlwaysFalse<Vector>,
                   "This size of a vector doesn't have impementation of rotate y yet" );
}

// ---------------------------------------------------------------------------------------------------------------------
template <>
inline Vec3 RotateY( const Vec3 &v, float angleRad )
{
    Vec3  result;
    float c = std::cos( angleRad );
    float s = std::sin( angleRad );

    result.x = v.x * c + v.z * s;
    result.y = v.y;
    result.z = -v.x * s + v.z * c;

    return result;
}

// ---------------------------------------------------------------------------------------------------------------------
template <class Vector>
constexpr inline Vector RotateZ( const Vector &, float )
{
    static_assert( Core::TypeIsAlwaysFalse<Vector>,
                   "This size of a vector doesn't have impementation of rotate z yet" );
}

// ---------------------------------------------------------------------------------------------------------------------
template <>
inline Vec3 RotateZ( const Vec3 &v, float angleRad )
{
    Vec3  result;
    float c = std::cos( angleRad );
    float s = std::sin( angleRad );

    result.x = v.x * c - v.y * s;
    result.y = v.x * s + v.y * c;
    result.z = v.z;

    return result;
}

// ---------------------------------------------------------------------------------------------------------------------
template <class Vector>
inline Vector &AddAssign( Vector &, const Vector & )
{
    static_assert( Core::TypeIsAlwaysFalse<Vector>,
                   "This size of a vector doesn't have impementation of add assign yet" );
}

// ---------------------------------------------------------------------------------------------------------------------
template <>
inline Vec3 &AddAssign( Vec3 &vA, const Vec3 &vB )
{
    vA.x += vB.x;
    vA.y += vB.y;
    vA.z += vB.z;
    return vA;
}

// ---------------------------------------------------------------------------------------------------------------------
template <>
inline iVec3 &AddAssign( iVec3 &vA, const iVec3 &vB )
{
    vA.x += vB.x;
    vA.y += vB.y;
    vA.z += vB.z;
    return vA;
}

// ---------------------------------------------------------------------------------------------------------------------
template <class Vector>
inline Vector &SubtractAssign( Vector &, const Vector & )
{
    static_assert( Core::TypeIsAlwaysFalse<Vector>,
                   "This size of a vector doesn't have impementation of subtract assign yet" );
}

// ---------------------------------------------------------------------------------------------------------------------
template <>
inline Vec3 &SubtractAssign( Vec3 &vA, const Vec3 &vB )
{
    vA.x -= vB.x;
    vA.y -= vB.y;
    vA.z -= vB.z;
    return vA;
}

// ---------------------------------------------------------------------------------------------------------------------
template <>
inline iVec3 &SubtractAssign( iVec3 &vA, const iVec3 &vB )
{
    vA.x -= vB.x;
    vA.y -= vB.y;
    vA.z -= vB.z;
    return vA;
}

// ---------------------------------------------------------------------------------------------------------------------
template <class Vector>
inline Vector Multiply( const Vector &, const Vector & )
{
    static_assert( Core::TypeIsAlwaysFalse<Vector>,
                   "This size of a vector doesn't have impementation of multiply yet" );
}

// ---------------------------------------------------------------------------------------------------------------------
template <>
inline Vec3 Multiply( const Vec3 &vA, const Vec3 &vB )
{
    Vec3 r;

    r.x = vA.x * vB.x;
    r.y = vA.y * vB.y;
    r.z = vA.z * vB.z;

    return r;
}

// ---------------------------------------------------------------------------------------------------------------------
template <>
inline iVec3 Multiply( const iVec3 &vA, const iVec3 &vB )
{
    iVec3 r;

    r.x = vA.x * vB.x;
    r.y = vA.y * vB.y;
    r.z = vA.z * vB.z;

    return r;
}

// ---------------------------------------------------------------------------------------------------------------------
template <class Vector>
constexpr inline Vector MultiplyScalar( const Vector &, const float )
{
    static_assert( Core::TypeIsAlwaysFalse<Vector>,
                   "This size of a vector doesn't have impementation of multiply by scalar yet" );
}

// --------------------------------------------------------------------------------------------------------------------
template <>
inline Vec3 MultiplyScalar( const Vec3 &vA, const float vB )
{
    Vec3 r;

    r.x = vA.x * vB;
    r.y = vA.y * vB;
    r.z = vA.z * vB;

    return r;
}

// --------------------------------------------------------------------------------------------------------------------
template <>
inline iVec3 MultiplyScalar( const iVec3 &vA, const float vB )
{
    iVec3 r;

    r.x = vA.x * vB;
    r.y = vA.y * vB;
    r.z = vA.z * vB;

    return r;
}

// ---------------------------------------------------------------------------------------------------------------------
inline Mat44 PerspectiveProjection( float fFovY, float width, float height, float fNear, float fFar )
{
    const float fInvAspectRatio = height / width;
    const float fFocal          = 1.0f / tan( fFovY * 0.5f );
    const float rangeInv        = 1.0f / ( fFar - fNear );
    Mat44       result          = {};

    result[ 0 ]  = fInvAspectRatio * fFocal;
    result[ 1 ]  = 0.f;
    result[ 2 ]  = 0.f;
    result[ 3 ]  = 0.f;
    result[ 4 ]  = 0.f;
    result[ 5 ]  = -fFocal;
    result[ 6 ]  = 0.f;
    result[ 7 ]  = 0.f;
    result[ 8 ]  = 0.f;
    result[ 9 ]  = 0.f;
    result[ 10 ] = fFar * rangeInv;
    result[ 11 ] = -fNear * fFar * rangeInv;
    result[ 12 ] = 0.f;
    result[ 13 ] = 0.f;
    result[ 14 ] = 1.f;
    result[ 15 ] = 0.f;

    return result;
}

} // namespace B33::Math
#endif // !B33_OPERATIONS_H
