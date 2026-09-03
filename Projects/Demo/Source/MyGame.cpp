#include "MyGame.hpp"
#include "Input/MouseButtonList.hpp"
#include "Input/Bind.h"
#include "Input/KeyList.hpp"
#include "MainWindow.hpp"
#include "MainCharacter.hpp"

MyGame::MyGame()
  : m_Game()
  , m_Paper( *this )
{
}

void MyGame::Initialize( ::B33::System::ComponentBridge &bridge )
{
    B33_TRACE( L"MyGame initialize" );
    m_Game.Initialize();
    auto input = bridge.QueryComponent<MainWindow>()->GetWindowInstance().GetInput();
    if ( auto lockedInput = input.lock() )
    {
        m_Paper.BindToInput(
            lockedInput,
            {
                { PaperController::UseActionMoveFront,
                  B33InputBind { .Type     = Keyboard,
                                 .Keyboard = B33KeyboardBind { .KeyState = EB33OnState::Continuous,
                                                              .KeyCode  = B33::App::B33_KEY_W } } },
                { PaperController::UseActionMoveLeft,
                  B33InputBind { .Type     = Keyboard,
                                 .Keyboard = B33KeyboardBind { .KeyState = EB33OnState::Continuous,
                                                              .KeyCode  = B33::App::B33_KEY_A } } },
                { PaperController::UseActionMoveRigth,
                  B33InputBind { .Type     = Keyboard,
                                 .Keyboard = B33KeyboardBind { .KeyState = EB33OnState::Continuous,
                                                              .KeyCode  = B33::App::B33_KEY_D } } },
                { PaperController::UseActionMoveBack,
                  B33InputBind { .Type     = Keyboard,
                                 .Keyboard = B33KeyboardBind { .KeyState = EB33OnState::Continuous,
                                                              .KeyCode  = B33::App::B33_KEY_S } } },
                { PaperController::UseActionMoveDown,
                  B33InputBind { .Type     = Keyboard,
                                 .Keyboard = B33KeyboardBind { .KeyState = EB33OnState::Continuous,
                                                              .KeyCode  = B33::App::B33_KEY_Q } } },
                { PaperController::UseActionMoveUp,
                  B33InputBind { .Type     = Keyboard,
                                 .Keyboard = B33KeyboardBind { .KeyState = EB33OnState::Continuous,
                                                              .KeyCode  = B33::App::B33_KEY_E } } },
                { PaperController::UseActionWalk,
                  B33InputBind { .Type     = Keyboard,
                                 .Keyboard = B33KeyboardBind { .KeyState = EB33OnState::Release,
                                                              .KeyCode  = B33::App::B33_KEY_LEFTSHIFT } } },
                { PaperController::UseActionSprint,
                  B33InputBind { .Type     = Keyboard,
                                 .Keyboard = B33KeyboardBind { .KeyState = EB33OnState::Continuous,
                                                              .KeyCode  = B33::App::B33_KEY_LEFTSHIFT } } },
                { PaperController::UseActionPushLowForce,
                  B33InputBind {
                      .Type     = Keyboard,
                      .Keyboard = B33KeyboardBind { .KeyState = EB33OnState::Press, .KeyCode = B33::App::B33_KEY_1 } } },
                { PaperController::UseActionPushMediumForce,
                  B33InputBind {
                      .Type     = Keyboard,
                      .Keyboard = B33KeyboardBind { .KeyState = EB33OnState::Press, .KeyCode = B33::App::B33_KEY_2 } } },
                { PaperController::UseActionPushHighForce,
                  B33InputBind {
                      .Type     = Keyboard,
                      .Keyboard = B33KeyboardBind { .KeyState = EB33OnState::Press, .KeyCode = B33::App::B33_KEY_3 } } },
                { PaperController::UseActionMouse, B33InputBind { .Type = Mouse } },
                { PaperController::UseActionPlaceBlock,
                  B33InputBind { .Type     = Keyboard,
                                 .Keyboard = B33KeyboardBind { .KeyState = EB33OnState::Press,
                                                              .KeyCode  = B33::App::B33_KEY_SPACE } } },
                { PaperController::UseActionPlaceBlock,
                  B33InputBind { .Type        = MouseButton,
                                 .MouseButton = B33MouseButtonBind { .ButtonState = EB33OnState::Press,
                                                                    .ButtonCode = B33::App::B33_LEFT_MOUSE_BUTTON } } },
                { PaperController::UseActionRemoveBlock,
                  B33InputBind { .Type = MouseButton,
                                 .MouseButton =
                                     B33MouseButtonBind { .ButtonState = EB33OnState::Press,
                                                         .ButtonCode  = B33::App::B33_RIGTH_MOUSE_BUTTON } } },
                { PaperController::UseActionRemoveBlock,
                  B33InputBind {
                      .Type     = Keyboard,
                      .Keyboard = B33KeyboardBind { .KeyState = EB33OnState::Press, .KeyCode = B33::App::B33_KEY_X } } },
                { PaperController::UseActionRotatePlacedTypeBackward,
                  B33InputBind {
                      .Type     = Keyboard,
                      .Keyboard = B33KeyboardBind { .KeyState = EB33OnState::Press, .KeyCode = B33::App::B33_KEY_B } } },
                { PaperController::UseActionRotatePlacedTypeForward,
                  B33InputBind {
                      .Type     = Keyboard,
                      .Keyboard = B33KeyboardBind { .KeyState = EB33OnState::Press, .KeyCode = B33::App::B33_KEY_V } } },

            } );
    }

    m_Paper.GetObject()->Initialize();
}

void MyGame::Update( float fDelta, ::B33::System::ComponentBridge &bridge )
{
    B33_TRACE( L"Game update" );
    m_Game.Update( fDelta );
}

void MyGame::Destroy( ::B33::System::ComponentBridge &bridge )
{
    B33_TRACE( L"MyGame destroy" );
}

const PlayablePaper &MyGame::GetMainCharacter() const
{
    return m_Paper;
}
