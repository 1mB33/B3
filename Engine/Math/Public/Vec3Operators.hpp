#if !defined( B33_VEC3_OPERATORS_HPP )
#    define B33_VEC3_OPERATORS_HPP

#    if defined( _B33_SSE )
#        include <tmmintrin.h>
#        include <smmintrin.h>
#    endif

#    include "Operations.hpp"
#    include "Vec3.hpp"

namespace B33::Math
{

// --------------------------------------------------------------------------------------------------------------------
constexpr float &Vec3::operator[]( usize uIndex )
{
    B33_ASSERT( uIndex < Size );

    return ( &x )[ uIndex ];
}

// --------------------------------------------------------------------------------------------------------------------
constexpr float Vec3::operator[]( usize uIndex ) const
{
    B33_ASSERT( uIndex < Size );

    return ( &x )[ uIndex ];
}

// --------------------------------------------------------------------------------------------------------------------
inline bool Vec3::operator==( const Vec3 &vB ) const
{
    using ::std::fabs;

    const float fEpsilon = 0.0001f;
    return ( fabs( this->x - vB.x ) < fEpsilon && fabs( this->y - vB.y ) < fEpsilon &&
             fabs( this->z - vB.z ) < fEpsilon )
               ? true
               : false;
}

// --------------------------------------------------------------------------------------------------------------------
inline Vec3 &Vec3::operator+=( const Vec3 &vB )
{
    return AddAssign( *this, vB );
}

// --------------------------------------------------------------------------------------------------------------------
inline Vec3 Vec3::operator+( const Vec3 &vB ) const
{
    Vec3 n( *this );
    return AddAssign( n, vB );
}

// --------------------------------------------------------------------------------------------------------------------
inline Vec3 Vec3::operator+( const iVec3 &vB ) const
{
    Vec3 n( *this );
    n = AddAssign( n, Vec3::ToVec( vB ) );
    return n;
}

// --------------------------------------------------------------------------------------------------------------------
inline Vec3 Vec3::operator+( const u32 vB ) const
{
    Vec3 n( *this );
    n = AddAssign( n, Vec3( vB, vB, vB ) );
    return n;
}

// --------------------------------------------------------------------------------------------------------------------
inline Vec3 Vec3::operator-( const Vec3 &vB ) const
{
    Vec3 n( *this );
    return SubtractAssign( n, vB );
}

// --------------------------------------------------------------------------------------------------------------------
inline Vec3 Vec3::operator*( const Vec3 &vB ) const
{
    Vec3 n( *this );
    return Multiply( n, vB );
}

// --------------------------------------------------------------------------------------------------------------------
inline Vec3 Vec3::operator*( const float vB ) const
{
    Vec3 n( *this );
    return MultiplyScalar( n, vB );
}

// --------------------------------------------------------------------------------------------------------------------
constexpr i32 &iVec3::operator[]( usize uIndex )
{
    B33_ASSERT( uIndex < Size );

    return ( &x )[ uIndex ];
}

// --------------------------------------------------------------------------------------------------------------------
constexpr i32 iVec3::operator[]( usize uIndex ) const
{
    B33_ASSERT( uIndex < Size );

    return ( &x )[ uIndex ];
}

// --------------------------------------------------------------------------------------------------------------------
inline iVec3 iVec3::operator+( const Vec3 &vB ) const
{
    iVec3 n( *this );
    return AddAssign( n, iVec3( vB ) );
}

// --------------------------------------------------------------------------------------------------------------------
inline iVec3 iVec3::operator-( const iVec3 &vB ) const
{
    iVec3 n( *this );
    return SubtractAssign( n, iVec3( vB ) );
}

// --------------------------------------------------------------------------------------------------------------------
inline iVec3 iVec3::operator*( const u32 vB ) const
{
    iVec3 n( *this );
    return Multiply( n, iVec3( vB, vB, vB ) );
}

} // namespace B33::Math
#endif // !B33_VEC3_OPERATORS_HPP
