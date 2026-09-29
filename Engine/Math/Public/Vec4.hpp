#if !defined( B33_VEC4_HPP )
#    define B33_VEC4_HPP

#    include <B33Core.h>

namespace B33::Math
{

// ---------------------------------------------------------------------------------------------------------------------
struct alignas( 16 ) Vec4
{
    static constexpr usize Size = 4;

  public:
    explicit Vec4( float x = 0, float y = 0, float z = 0, float w = 0 ) noexcept
      : x( x )
      , y( y )
      , z( z )
      , w( w )
    {
    }

#    if defined( _B33_SSE )
    explicit Vec4( __m128 xyzw ) noexcept
      : xyzw( xyzw )
    {
    }
#    endif

    ~Vec4() noexcept = default;

  public:
    Vec4( Vec4 && ) noexcept = default;
    Vec4( const Vec4 & )     = default;

    Vec4 &operator=( const Vec4 & ) noexcept = default;
    Vec4 &operator=( Vec4 && ) noexcept      = default;

  public:
    union
    {
        struct alignas( 16 )
        {
            float x;
            float y;
            float z;
            float w;
        };
#    if defined( _B33_SSE )
        __m128 xyzw;
#    endif
    };

  public:
    template <typename Vector>
    static Vec4 ToVec4( Vector v ) noexcept
    {
        return Vec4( v.x, v.y, v.z, 0.f );
    }

  public:
    constexpr float operator[]( usize uIndex ) const
    {
        B33_ASSERT( uIndex < Size );

        return ( &x )[ uIndex ];
    }

    constexpr float &operator[]( usize uIndex )
    {
        B33_ASSERT( uIndex < Size );

        return ( &x )[ uIndex ];
    }
};

// ---------------------------------------------------------------------------------------------------------------------
struct alignas( 16 ) iVec4
{
    static constexpr usize Size = 4;

  public:
    explicit iVec4( i32 x = 0, i32 y = 0, i32 z = 0, i32 w = 0 ) noexcept
      : x( x )
      , y( y )
      , z( z )
      , w( w )
    {
    }

#    if defined( _B33_SSE )
    explicit iVec4( __m128 xyzw ) noexcept
      : xyzw( xyzw )
    {
    }
#    endif

    ~iVec4() noexcept = default;

  public:
    iVec4( iVec4 && ) noexcept = default;
    iVec4( const iVec4 & )     = default;

    iVec4 &operator=( const iVec4 & ) noexcept = default;
    iVec4 &operator=( iVec4 && ) noexcept      = default;

  public:
    union
    {
        struct alignas( 16 )
        {
            i32 x;
            i32 y;
            i32 z;
            i32 w;
        };
#    if defined( _B33_SSE )
        __m128 xyzw;
#    endif
    };

  public:
    template <typename Vector>
    static iVec4 ToiVec4( Vector v ) noexcept
    {
        return iVec4( v.x, v.y, v.z, 0.f );
    }

  public:
    constexpr i32 operator[]( usize uIndex ) const
    {
        B33_ASSERT( uIndex < Size );

        return ( &x )[ uIndex ];
    }

    constexpr i32 &operator[]( usize uIndex )
    {
        B33_ASSERT( uIndex < Size );

        return ( &x )[ uIndex ];
    }
};

} // namespace B33::Math
#endif // !B33_VEC4_HPP
