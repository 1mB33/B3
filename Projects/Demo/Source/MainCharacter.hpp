#pragma once

#include "Game.hpp"

#include "Input/Bind.h"
#include "Input/ControllerObject.hpp"
#include "Input/UserInput.hpp"
#include "Operations.hpp"
#include "Primitives/Camera.hpp"
#include "Raycaster/Rays.hpp"
#include "Input/Action.hpp"
#include "Input/Playable.hpp"

class PaperCharacter : public ::B33::Rendering::Camera
{
  public:
    template <class... U>
    explicit PaperCharacter( Game &vg, U &&...args )
      : m_g( vg )
      , Camera( ::std::forward<U>( args )... )
      , m_fSpeed( m_fWalk )
    {
    }

  public:
    void PlaceBlock( const float )
    {
        B33::Math::Vec3 rot     = this->GetRotation();
        B33::Math::Vec3 lookDir = B33::Math::Normalize(
            B33::Math::RotateY( B33::Math::RotateX( B33::Math::Vec3 { 0.f, 0.f, 1.f }, rot.x ), rot.y ) );

        B33::Rendering::HitResult hr =
            ::B33::Rendering::MarchTheRay( m_g.GetWorld().get(), this->GetPosition(), lookDir, 10 );

        if ( hr.bHit )
        {
            m_g.GenerateCube( B33::Math::iVec3( hr.iHitCoords + hr.Normal ) );
        }
    }

    void RemoveBlock( const float )
    {
        B33::Math::Vec3 rot     = this->GetRotation();
        B33::Math::Vec3 lookDir = ::B33::Math::Normalize(
            ::B33::Math::RotateY( ::B33::Math::RotateX( ::B33::Math::Vec3 { 0.f, 0.f, 1.f }, rot.x ), rot.y ) );

        B33::Rendering::HitResult hr =
            ::B33::Rendering::MarchTheRay( m_g.GetWorld().get(), this->GetPosition(), lookDir, 10 );

        if ( hr.bHit )
            m_g.RemoveCube( m_g.GetIdFromPos( hr.iHitCoords ) );
    }

    void Push( const float, const float fForceMul )
    {
        B33::Math::Vec3 rot     = this->GetRotation();
        B33::Math::Vec3 lookDir = ::B33::Math::Normalize(
            ::B33::Math::RotateY( ::B33::Math::RotateX( ::B33::Math::Vec3 { 0.f, 0.f, 1.f }, rot.x ), rot.y ) );

        ::B33::Rendering::HitResult hr =
            ::B33::Rendering::MarchTheRay( m_g.GetWorld().get(), this->GetPosition(), lookDir, 10 );
        ::B33::Math::Vec3 pushDir = ::B33::Math::Normalize( this->GetPosition() - ::B33::Math::Vec3( hr.iHitCoords ) );
        if ( hr.bHit )
            m_g.PushCube( m_g.GetIdFromPos( hr.iHitCoords ), pushDir, fForceMul );
    }

    void MoveForwardBackwards( const float fDelta, const float fDir )
    {
        B33::Math::Rot3 rot     = this->GetRotation();
        B33::Math::Vec3 lookDir = ::B33::Math::RotateY( ::B33::Math::Vec3 { 0.f, 0.f, 1.f }, rot.y );

        this->AddPositon( lookDir * fDir * ( fDelta * m_fSpeed ) );
    }

    void Strafe( const float fDelta, float fDir )
    {
        B33::Math::Rot3 rot = this->GetRotation();
        B33::Math::Vec3 lookDir =
            ::B33::Math::RotateY( ::B33::Math::Vec3 { 0.f, 0.f, 1.f }, rot.y + ( 90.f * ::B33::Math::B33_DEG_TO_RAD ) );

        this->AddPositon( lookDir * fDir * ( fDelta * m_fSpeed ) );
    }

    void MouseMove( const float, int32_t fX, int32_t fY )
    {
        this->AddRotation( B33::Math::Rot3 { 0.00085f * fY, 0.00085f * fX, 0.f } );
    }

    void MoveVertical( const float fDelta, const float dir )
    {
        this->AddPositon( ::B33::Math::Vec3 { 0.f, dir * ( fDelta * m_fSpeed ), 0.f } );
    }

    void Move( const float fDelta, const B33::Math::Vec3 &dir )
    {
        this->AddPositon( dir * ( fDelta * m_fSpeed ) );
    }

    void ActivateSprint( const float )
    {
        m_fSpeed = m_fSprint;
    }

    void ActivateWalk( const float )
    {
        m_fSpeed = m_fWalk;
    }

  private:
    Game &m_g;

    uint32_t m_uColor;

    static constexpr float m_fWalk   = 0.1;
    static constexpr float m_fSprint = 0.3;
    float                  m_fSpeed  = -1.f;
};

class PaperController : public B33::App::ControllerObject
{
    using Action        = ::B33::App::Action;
    using ActionFactory = ::B33::App::ActionFactory;

  public:
    const inline static Action UseActionMoveRigth =
        ActionFactory::CreateKeyboardAction<PaperCharacter, &PaperCharacter::Strafe, 0.1f>();

    const inline static Action UseActionMoveLeft =
        ActionFactory::CreateKeyboardAction<PaperCharacter, &PaperCharacter::Strafe, -0.1f>();

    const inline static Action UseActionMoveUp =
        ActionFactory::CreateKeyboardAction<PaperCharacter, &PaperCharacter::MoveVertical, 0.1f>();

    const inline static Action UseActionMoveDown =
        ActionFactory::CreateKeyboardAction<PaperCharacter, &PaperCharacter::MoveVertical, -0.1f>();

    const inline static Action UseActionPlaceBlock =
        ActionFactory::CreateKeyboardAction<PaperCharacter, &PaperCharacter::PlaceBlock>();

    const inline static Action UseActionRemoveBlock =
        ActionFactory::CreateKeyboardAction<PaperCharacter, &PaperCharacter::RemoveBlock>();

    const inline static Action UseActionPushLowForce =
        ActionFactory::CreateKeyboardAction<PaperCharacter, &PaperCharacter::Push, 10.f>();

    const inline static Action UseActionPushMediumForce =
        ActionFactory::CreateKeyboardAction<PaperCharacter, &PaperCharacter::Push, 50.f>();

    const inline static Action UseActionPushHighForce =
        ActionFactory::CreateKeyboardAction<PaperCharacter, &PaperCharacter::Push, 100.f>();

    const inline static Action UseActionWalk =
        ActionFactory::CreateKeyboardAction<PaperCharacter, &PaperCharacter::ActivateWalk>();

    const inline static Action UseActionSprint =
        ActionFactory::CreateKeyboardAction<PaperCharacter, &PaperCharacter::ActivateSprint>();

    const inline static Action UseActionMoveBack =
        ActionFactory::CreateKeyboardAction<PaperCharacter, &PaperCharacter::MoveForwardBackwards, -0.1f>();

    const inline static Action UseActionMoveFront =
        ActionFactory::CreateKeyboardAction<PaperCharacter, &PaperCharacter::MoveForwardBackwards, 0.1f>();

    const inline static Action UseActionMouse =
        ActionFactory::CreateMouseAction<PaperCharacter, &PaperCharacter::MouseMove>();
};

typedef ::B33::App::Playable<PaperCharacter, PaperController> PlayablePaper;
