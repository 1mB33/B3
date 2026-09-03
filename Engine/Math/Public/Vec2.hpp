#if !defined(B22_VEC2_H)
#define B22_VEC2_H

#include "B33Core.h"

namespace B33::Math
{

// ---------------------------------------------------------------------------------------------------------------------
struct alignas( 16 ) Vec2
{
    static constexpr size_t Size = 2;

  public:
    Vec2()
      : x( 0.f )
      , y( 0.f )
    {
    }

    explicit Vec2( float x, float y = 0. ) noexcept
      : x( x )
      , y( y )
    {
    }

    template <typename Vector>
    explicit Vec2( Vector v )
      : x( v.x )
      , y( v.y )
    {
    }

    ~Vec2() noexcept = default;

  public:
    Vec2( Vec2 && ) noexcept = default;
    Vec2( const Vec2 & )     = default;

    Vec2 &operator=( const Vec2 & ) noexcept = default;
    Vec2 &operator=( Vec2 && ) noexcept      = default;

  public:
    float x;
    float y;

  public:
    template <typename Vector>
    static Vec2 ToVec( Vector v )
    {
        return Vec2( v.x, v.y );
    }
};

// ---------------------------------------------------------------------------------------------------------------------
struct alignas( 16 ) iVec2
{
    static constexpr size_t Size = 2;

  public:
    iVec2()
      : x( 0 )
      , y( 0 )
    {
    }

    explicit iVec2( int32_t x, int32_t y = 0 )
      : x( x )
      , y( y )
    {
    }

    explicit iVec2( Vec2 v )
      : x( v.x )
      , y( v.y )
    {
    }

    ~iVec2() = default;

  public:
    iVec2( iVec2 && ) noexcept = default;
    iVec2( const iVec2 & )     = default;

    iVec2 &operator=( const iVec2 & ) noexcept = default;
    iVec2 &operator=( iVec2 && ) noexcept      = default;

  public:
    int32_t x;
    int32_t y;

  public:
    template <typename Vector>
    static iVec2 ToVec( Vector v )
    {
        return iVec2( v.x, v.y );
    }
};

} // namespace B22::Math
#endif // !B22_VEC2_H
