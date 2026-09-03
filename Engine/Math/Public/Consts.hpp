#if !defined(B33_MATH_CONSTS_H)
#define B33_MATH_CONSTS_H

namespace B33::Math
{

constexpr float B33_DEG_TO_RAD_RATIO = 0.01745329f;
constexpr float B33_PI_RAD_RATIO     = 180.f * 0.01745329f;

constexpr float B33_DEG_TO_RAD( float deg )
{
    return B33_DEG_TO_RAD_RATIO * deg;
}

} // namespace B33::Math
#endif // !B33_MATH_CONSTS_H
