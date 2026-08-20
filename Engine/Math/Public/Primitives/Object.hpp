#ifndef B33_OBJECT_H
#define B33_OBJECT_H

#include "Rot.hpp"
#include "Vec3.hpp"

namespace B33::Math
{

/**
 * Holds positon of an object and rotation in radians.
 * */
class alignas( 16 ) WorldObject
{
  public:
    explicit WorldObject( Vec3 positon = Vec3 { 0.f, 0.f, 0.f }, Rot3 rotation = Vec3 { 0.f, 0.f, 0.f } )
      : m_vPosition( positon )
      , m_vRotation( rotation )
    {
    }

    ~WorldObject() = default;

  public:
    WorldObject( WorldObject && )      = default;
    WorldObject( const WorldObject & ) = default;

    WorldObject &operator=( const WorldObject & ) noexcept = default;
    WorldObject &operator=( WorldObject && ) noexcept      = default;

    // Getters // -----------------------------------------------------------------------------------------------------
  public:
    const Vec3 &GetPosition() const
    {
        return m_vPosition;
    }

    const Rot3 &GetRotation() const
    {
        return m_vRotation;
    }

    // Setters // -----------------------------------------------------------------------------------------------------
  public:
    __B33_API void SetPositon( Vec3 pos );

    __B33_API void SetRotation( Rot3 rot );

    // Methods // -----------------------------------------------------------------------------------------------------
  public:
    __B33_API void AddPositon( const Vec3 &pos );

    __B33_API void AddRotation( const Rot3 &rot );

  private:
    Vec3 m_vPosition;
    Rot3 m_vRotation;
};

} // namespace B33::Math
#endif // !B33_OBJECT_H
