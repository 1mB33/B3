#if !defined( B33_CAMERA_HPP )
#    define B33_CAMERA_HPP

#    include <B33Math.hpp>
#    include "Primitives/Object.hpp"

namespace B33::Rendering
{

/**
 * Basic camera.
 * Fov is horizontal, stored in degrees.
 * */
class Camera : public ::B33::Math::WorldObject
{
    using Mat44 = ::B33::Math::Mat44;

  public:
    Camera( float fFov = 90.f )
      : m_fFov( fFov )
    {
    }

    ~Camera() = default;

  public:
    void IncreaseFov( float f )
    {
        m_fFov += f;
    }

    void SetFov( float f )
    {
        m_fFov = f;
    }

  public:
    float GetFov() const
    {
        return m_fFov;
    }

  public:
    __B33_API Mat44 GenerateLookAt();

  private:
    float m_fFov;
};

} // namespace B33::Rendering
#endif // !B33_CAMERA_HPP
