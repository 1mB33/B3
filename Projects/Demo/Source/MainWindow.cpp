#include "B33Core.h"
#include "MainWindow.hpp"

#include "EntryPoints/DefaultSystemEntryPoint.hpp"
#include "WindowMaster.hpp"

void MainWindow::Initialize( ::B33::System::ComponentBridge & )
{
    B33_TRACE( L"MainWindow initialize" );
    m_WindowInstance.Create();
    m_WindowPuppet.BindToInput(
        m_WindowInstance.GetInput().lock(),
        {
            { WindowMasterController::UseActionSetWindowMode,
              B33InputBind {
                  .Type     = Keyboard,
                  .Keyboard = B33KeyboardBind { .KeyState = EB33OnState::Press, .KeyCode = B33::App::B33_KEY_F1 } } },
            { WindowMasterController::UseActionSetWindowBorderless,
              B33InputBind {
                  .Type     = Keyboard,
                  .Keyboard = B33KeyboardBind { .KeyState = EB33OnState::Press, .KeyCode = B33::App::B33_KEY_F2 } } },
            { WindowMasterController::UseActionExit,
              B33InputBind {
                  .Type     = Keyboard,
                  .Keyboard = B33KeyboardBind { .KeyState = EB33OnState::Press, .KeyCode = B33::App::B33_KEY_ESC } } },
        } );

    m_WindowInstance.Show();
    m_WindowInstance.GetInput().lock()->StartCapturing();
}

void MainWindow::Update( float fDelta, ::B33::System::ComponentBridge & )
{
    B33_TRACE( L"Window update" );
    m_WindowInstance.Update( fDelta );
    m_WindowInstance.GetInput().lock()->Update( fDelta );
}

void MainWindow::Destroy( ::B33::System::ComponentBridge & )
{
    B33_TRACE( L"MainWindow destroy" );
    m_WindowInstance.GetInput().lock()->StopCapturing();
    m_WindowInstance.Destroy();
}
