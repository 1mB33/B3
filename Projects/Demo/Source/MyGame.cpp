#include "MyGame.hpp"
#include "Input/Bind.h"
#include "Input/KeyList.hpp"
#include "MainWindow.hpp"

void MyGame::Initialize( ::B33::System::ComponentBridge &bridge )
{
    m_Game.Initialize();
    auto input = bridge.QueryComponent<MainWindow>().GetWindowInstance().GetInput();
    if ( auto lockedInput = input.lock() )
    {
        m_Paper.BindToInput(
            lockedInput,
            {
                { PaperController::UseActionMoveFront,
                  AbInputBind { .Type     = Keyboard,
                                .Keyboard = AbKeyboardBind { .KeyState = EAbOnState::Continuous,
                                                             .KeyCode  = B33::App::B33_KEY_W } } },
                { PaperController::UseActionMoveLeft,
                  AbInputBind { .Type     = Keyboard,
                                .Keyboard = AbKeyboardBind { .KeyState = EAbOnState::Continuous,
                                                             .KeyCode  = B33::App::B33_KEY_A } } },
                { PaperController::UseActionMoveRigth,
                  AbInputBind { .Type     = Keyboard,
                                .Keyboard = AbKeyboardBind { .KeyState = EAbOnState::Continuous,
                                                             .KeyCode  = B33::App::B33_KEY_D } } },
                { PaperController::UseActionMoveBack,
                  AbInputBind { .Type     = Keyboard,
                                .Keyboard = AbKeyboardBind { .KeyState = EAbOnState::Continuous,
                                                             .KeyCode  = B33::App::B33_KEY_S } } },
                { PaperController::UseActionMoveDown,
                  AbInputBind { .Type     = Keyboard,
                                .Keyboard = AbKeyboardBind { .KeyState = EAbOnState::Continuous,
                                                             .KeyCode  = B33::App::B33_KEY_Q } } },
                { PaperController::UseActionMoveUp,
                  AbInputBind { .Type     = Keyboard,
                                .Keyboard = AbKeyboardBind { .KeyState = EAbOnState::Continuous,
                                                             .KeyCode  = B33::App::B33_KEY_E } } },
                { PaperController::UseActionPushLowForce,
                  AbInputBind {
                      .Type     = Keyboard,
                      .Keyboard = AbKeyboardBind { .KeyState = EAbOnState::Press, .KeyCode = B33::App::B33_KEY_1 } } },
                { PaperController::UseActionPushMediumForce,
                  AbInputBind {
                      .Type     = Keyboard,
                      .Keyboard = AbKeyboardBind { .KeyState = EAbOnState::Press, .KeyCode = B33::App::B33_KEY_2 } } },
                { PaperController::UseActionPushHighForce,
                  AbInputBind {
                      .Type     = Keyboard,
                      .Keyboard = AbKeyboardBind { .KeyState = EAbOnState::Press, .KeyCode = B33::App::B33_KEY_3 } } },
                { PaperController::UseActionMouse, AbInputBind { .Type = Mouse } },
                { PaperController::UseActionPlaceBlock,
                  AbInputBind { .Type     = Keyboard,
                                .Keyboard = AbKeyboardBind { .KeyState = EAbOnState::Press,
                                                             .KeyCode  = B33::App::B33_KEY_SPACE } } },
                { PaperController::UseActionPlaceBlock,
                  AbInputBind { .Type        = MouseButton,
                                .MouseButton = AbMouseButtonBind { .ButtonState = EAbOnState::Press,
                                                                   .ButtonCode  = B33::App::B33_LEFT_MOUSE_BUTTON } } },
                { PaperController::UseActionRemoveBlock,
                  AbInputBind { .Type        = MouseButton,
                                .MouseButton = AbMouseButtonBind { .ButtonState = EAbOnState::Press,
                                                                   .ButtonCode = B33::App::B33_RIGTH_MOUSE_BUTTON } } },
                { PaperController::UseActionRemoveBlock,
                  AbInputBind {
                      .Type     = Keyboard,
                      .Keyboard = AbKeyboardBind { .KeyState = EAbOnState::Press, .KeyCode = B33::App::B33_KEY_X } } },
                { PaperController::UseActionRotatePlacedTypeBackward,
                  AbInputBind {
                      .Type     = Keyboard,
                      .Keyboard = AbKeyboardBind { .KeyState = EAbOnState::Press, .KeyCode = B33::App::B33_KEY_B } } },
                { PaperController::UseActionRotatePlacedTypeForward,
                  AbInputBind {
                      .Type     = Keyboard,
                      .Keyboard = AbKeyboardBind { .KeyState = EAbOnState::Press, .KeyCode = B33::App::B33_KEY_V } } },

            } );
    }

    m_Paper.GetObject().SetRotation( ::B33::Math::Vec3 { -0.5f, 1.25f, 0.f } );
    m_Paper.GetObject().SetPositon( ::B33::Math::Vec3 { 14.5f, 2.25f, 25.f } );
}

void MyGame::Update( float fDelta )
{
    B33_TRACE( L"Game is updated, delta = %f", fDelta );
    m_Game.Update( fDelta );
}

void MyGame::Destroy( ::B33::System::ComponentBridge &bridge ) {}
