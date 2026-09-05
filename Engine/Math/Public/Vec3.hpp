#if !defined( B33_VEC3_HPP )
#    define B33_VEC3_HPP

#    include <B33Core.h>

namespace B33::Math
{

// ---------------------------------------------------------------------------------------------------------------------
struct alignas( 16 ) Vec3
{
    static constexpr usize Size = 3;

  public:
    Vec3() noexcept
      : x( 0.f )
      , y( 0.f )
      , z( 0.f )
    {
    }

    explicit Vec3( float x, float y = 0., float z = 0.f ) noexcept
      : x( x )
      , y( y )
      , z( z )
    {
    }

    template <typename Vector>
    explicit Vec3( Vector v ) noexcept
      : x( v.x )
      , y( v.y )
      , z( v.z )
    {
    }

    ~Vec3() noexcept = default;

  public:
    Vec3( Vec3 && ) noexcept = default;
    Vec3( const Vec3 & )     = default;

    Vec3 &operator=( const Vec3 & ) noexcept = default;
    Vec3 &operator=( Vec3 && ) noexcept      = default;

  public:
    float x;
    float y;
    float z;
    float _Padding;

  public:
    template <typename Vector>
    static Vec3 ToVec( Vector v ) noexcept
    {
        return Vec3( v.x, v.y, v.z );
    }

  public:
    constexpr float &operator[]( usize uIndex );

    constexpr float operator[]( usize uIndex ) const;

    inline bool operator==( const Vec3 &vB ) const;

    inline Vec3 &operator+=( const Vec3 &vB );

    inline Vec3 operator+( const Vec3 &vB ) const;

    inline Vec3 operator+( const struct iVec3 &vB ) const;

    inline Vec3 operator+( const u32 vB ) const;

    inline Vec3 operator-( const Vec3 &vB ) const;

    inline Vec3 operator*( const Vec3 &vB ) const;

    inline Vec3 operator*( const float vB ) const;
};

// ---------------------------------------------------------------------------------------------------------------------
struct alignas( 16 ) iVec3
{
    static constexpr usize Size = 3;

  public:
    iVec3() noexcept
      : x( 0 )
      , y( 0 )
      , z( 0 )
    {
    }

    explicit iVec3( i32 x, i32 y = 0, i32 z = 0 ) noexcept
      : x( x )
      , y( y )
      , z( z )
    {
    }

    explicit iVec3( Vec3 v ) noexcept
      : x( v.x )
      , y( v.y )
      , z( v.z )
    {
    }

    ~iVec3() noexcept = default;

  public:
    iVec3( iVec3 && ) noexcept = default;
    iVec3( const iVec3 & )     = default;

    iVec3 &operator=( const iVec3 & ) noexcept = default;
    iVec3 &operator=( iVec3 && ) noexcept      = default;

  public:
    int32_t x;
    int32_t y;
    int32_t z;
    int32_t _Padding;

  public:
    template <typename Vector>
    static iVec3 ToVec( Vector v ) noexcept
    {
        return iVec3( v.x, v.y, v.z );
    }

  public:
    constexpr bool operator==( const iVec3 &other ) const noexcept
    {
        return this->x == other.x && this->y == other.y && this->z == other.z;
    }

    constexpr i32 operator[]( usize uIndex ) const
    {
        B33_ASSERT( uIndex < Size );

        if ( uIndex == 0 )
        {
            return x;
        }
        if ( uIndex == 1 )
        {
            return y;
        }
        return z;
    }

    constexpr i32 &operator[]( usize uIndex )
    {
        B33_ASSERT( uIndex < Size );

        if ( uIndex == 0 )
        {
            return x;
        }
        if ( uIndex == 1 )
        {
            return y;
        }
        return z;
    }

    inline iVec3 operator+( const Vec3 &vB ) const;

    inline iVec3 operator-( const iVec3 &vB ) const;

    inline iVec3 operator*( const u32 vB ) const;
};

} // namespace B33::Math
#endif // !B33_VEC3_HPP
