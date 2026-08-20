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

class MyGame;

class PaperCharacter : public ::B33::Rendering::Camera
{
  public:
    template <class... U>
    explicit PaperCharacter( MyGame &vg, U &&...args )
      : m_g( vg )
      , Camera( ::std::forward<U>( args )... )
      , m_fSpeed( m_fWalk )
    {
    }

  public:
    void Initialize();

  public:
    void PlaceBlock( const float );

    void RemoveBlock( const float );

    void Push( const float, const float fForceMul );

    void MoveForwardBackwards( const float fDelta, const float fDir );

    void Strafe( const float fDelta, float fDir );

    void MouseMove( const float, int32_t fX, int32_t fY );

    void MoveVertical( const float fDelta, const float dir );

    void Move( const float fDelta, const B33::Math::Vec3 &dir );

    void ActivateSprint( const float );

    void ActivateWalk( const float );

    void RotatePlaceTypeBackward( const float );

    void RotatePlaceTypeForward( const float );

  private:
    MyGame &m_g;

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
