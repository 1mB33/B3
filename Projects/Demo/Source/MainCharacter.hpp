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
    void Initialize()
    {
        this->SetRotation( ::B33::Math::Vec3 { -0.5f, 1.25f, 0.f } );
        this->SetPositon( ::B33::Math::Vec3 { 14.5f, 2.25f, 25.f } );
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
            auto id = m_g.GetIdFromPos( hr.iHitCoords );
            if ( id + 1 )
            {
                const auto halfSize =
                    B33::Math::Vec3( 2.0f, 2.0f, 2.0f ) *
                    m_g.GetWorld()->GetStoredObjects().GetHalfSize( m_g.GetIdFromPos( hr.iHitCoords ) ) * hr.Normal;
                B33_TRACE( L"GenerateCube, placing on top of cube with halfsizes %f %f %f",
                           halfSize.x,
                           halfSize.y,
                           halfSize.z );
                m_g.GenerateCube(
                    B33::Math::iVec3( hr.iHitCoords + halfSize ),
                    ::B33::Math::Vec3( m_fPlacedType + 0.1f, m_fPlacedType + 0.1f, m_fPlacedType + 0.1f ) );
            }
            else
            {
                m_g.GenerateCube(
                    B33::Math::iVec3( hr.iHitCoords + hr.Normal ),
                    ::B33::Math::Vec3( m_fPlacedType + 0.1f, m_fPlacedType + 0.1f, m_fPlacedType + 0.1f ) );
            }
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

    void RotatePlaceTypeBackward( const float )
    {
        m_fPlacedType = std::max( m_fPlacedType - 0.25f, 0.f );
    }

    void RotatePlaceTypeForward( const float )
    {
        m_fPlacedType = std::min( m_fPlacedType + 0.25f, 10.f );
    }

  private:
    Game &m_g;

    uint32_t m_uColor;

    static constexpr float m_fWalk       = 0.1;
    static constexpr float m_fSprint     = 0.3;
    float                  m_fSpeed      = -1.f;
    float                  m_fPlacedType = 0;
};

typedef ::B33::App::Playable<class PaperCharacter, class PaperController> PlayablePaper;

class PaperController : public B33::App::ControllerObject
{
    using Action        = ::B33::App::Action;
    using ActionFactory = ::B33::App::ActionFactory;

  public:
    const inline static Action UseActionMoveRigth =
        ActionFactory::CreateKeyboardAction<PlayablePaper, &PaperCharacter::Strafe, 0.1f>();

    const inline static Action UseActionMoveLeft =
        ActionFactory::CreateKeyboardAction<PlayablePaper, &PaperCharacter::Strafe, -0.1f>();

    const inline static Action UseActionMoveUp =
        ActionFactory::CreateKeyboardAction<PlayablePaper, &PaperCharacter::MoveVertical, 0.1f>();

    const inline static Action UseActionMoveDown =
        ActionFactory::CreateKeyboardAction<PlayablePaper, &PaperCharacter::MoveVertical, -0.1f>();

    const inline static Action UseActionPlaceBlock =
        ActionFactory::CreateKeyboardAction<PlayablePaper, &PaperCharacter::PlaceBlock>();

    const inline static Action UseActionRemoveBlock =
        ActionFactory::CreateKeyboardAction<PlayablePaper, &PaperCharacter::RemoveBlock>();

    const inline static Action UseActionPushLowForce =
        ActionFactory::CreateKeyboardAction<PlayablePaper, &PaperCharacter::Push, 10.f>();

    const inline static Action UseActionPushMediumForce =
        ActionFactory::CreateKeyboardAction<PlayablePaper, &PaperCharacter::Push, 50.f>();

    const inline static Action UseActionPushHighForce =
        ActionFactory::CreateKeyboardAction<PlayablePaper, &PaperCharacter::Push, 100.f>();

    const inline static Action UseActionWalk =
        ActionFactory::CreateKeyboardAction<PlayablePaper, &PaperCharacter::ActivateWalk>();

    const inline static Action UseActionSprint =
        ActionFactory::CreateKeyboardAction<PlayablePaper, &PaperCharacter::ActivateSprint>();

    const inline static Action UseActionMoveBack =
        ActionFactory::CreateKeyboardAction<PlayablePaper, &PaperCharacter::MoveForwardBackwards, -0.1f>();

    const inline static Action UseActionMoveFront =
        ActionFactory::CreateKeyboardAction<PlayablePaper, &PaperCharacter::MoveForwardBackwards, 0.1f>();

    const inline static Action UseActionMouse =
        ActionFactory::CreateMouseAction<PlayablePaper, &PaperCharacter::MouseMove>();

    const inline static Action UseActionRotatePlacedTypeForward =
        ActionFactory::CreateKeyboardAction<PlayablePaper, &PaperCharacter::RotatePlaceTypeForward>();

    const inline static Action UseActionRotatePlacedTypeBackward =
        ActionFactory::CreateKeyboardAction<PlayablePaper, &PaperCharacter::RotatePlaceTypeBackward>();
};
