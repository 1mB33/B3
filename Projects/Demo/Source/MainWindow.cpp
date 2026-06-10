#include "MainWindow.hpp"

#include "EntryPoints/DefaultSystemEntryPoint.hpp"
#include "WindowMaster.hpp"

void MainWindow::Initialize()
{
    m_WindowInstance.Create();
    m_WindowPuppet.BindToInput( m_WindowInstance.GetInput().lock(),
                                {
                                    { WindowMasterController::UseActionSetWindowMode,
                                      AbInputBind { .Type     = Keyboard,
                                                    .Keyboard = AbKeyboardBind { .KeyState = EAbOnState::Press,
                                                                                 .KeyCode  = B33::App::B33_KEY_F1 } } },
                                    { WindowMasterController::UseActionSetWindowBorderless,
                                      AbInputBind { .Type     = Keyboard,
                                                    .Keyboard = AbKeyboardBind { .KeyState = EAbOnState::Press,
                                                                                 .KeyCode  = B33::App::B33_KEY_F2 } } },
                                    { WindowMasterController::UseActionExit,
                                      AbInputBind { .Type     = Keyboard,
                                                    .Keyboard = AbKeyboardBind { .KeyState = EAbOnState::Press,
                                                                                 .KeyCode = B33::App::B33_KEY_ESC } } },
                                } );

    m_WindowInstance.GetInput().lock()->StartCapturing();
}

void MainWindow::Update( float fDelta )
{
    B33_TRACE( L"MainWindow is updated, delta = %f", fDelta );
    m_WindowInstance.Update( fDelta );
    m_WindowInstance.GetInput().lock()->Update( fDelta );
}

void MainWindow::Destroy()
{
    m_WindowInstance.GetInput().lock()->StopCapturing();
    m_WindowInstance.Destroy();
}
