#if !defined( B33_MATH_CONSTS_HPP )
#    define B33_MATH_CONSTS_HPP

namespace B33::Math
{

constexpr float B33_PI               = 3.141592653f;
constexpr float B33_DEG_TO_RAD_RATIO = 0.01745329f;

constexpr float B33_RAD_TO_DEG( float rad )
{
    return rad / B33_DEG_TO_RAD_RATIO;
}

constexpr float B33_DEG_TO_RAD( float deg )
{
    return B33_DEG_TO_RAD_RATIO * deg;
}

} // namespace B33::Math
#endif // !B33_MATH_CONSTS_HPP
