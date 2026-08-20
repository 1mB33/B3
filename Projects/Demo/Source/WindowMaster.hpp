#pragma once

#include "EmptyCanvas.hpp"
#include "Input/ControllerObject.hpp"
#include "Window/WindowPolicy/BorderlessGameSystemPolicy.hpp"
#include "Window/WindowPolicy/GameSystemPolicy.hpp"
#include "Input/Action.hpp"
#include "Input/Playable.hpp"

class WindowMaster
{
  public:
    explicit WindowMaster( ::B33::App::EmptyCanvas<true, B33::App::DefaultGameSystemWindowPolicy> &window )
      : m_Window( window )
    {
    }

  public:
    void SetWindow( const float )
    {
        ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info, L"Changing window display policy to window" );
        m_Window.ChangePolicy<B33::App::DefaultGameSystemWindowPolicy>();
    }

    void SetBorderless( const float )
    {
        ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info,
                                               L"Changing window display policy to borderless" );
        m_Window.ChangePolicy<B33::App::BorderlessGameSystemPolicy>();
    }

    void ExitGame( const float )
    {
        ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info, L"Closing game" );
        ::B33::App::AppStatus::Get().SendExitSignal();
    }

  private:
    B33::App::EmptyCanvas<true, B33::App::DefaultGameSystemWindowPolicy> &m_Window;
};

typedef ::B33::App::Playable<class WindowMaster, class WindowMasterController> WindowMasterPuppet;

class WindowMasterController : public B33::App::ControllerObject
{
    using Action        = ::B33::App::Action;
    using ActionFactory = ::B33::App::ActionFactory;

  public:
    const inline static Action UseActionSetWindowMode =
        ActionFactory::CreateKeyboardAction<WindowMasterPuppet, &WindowMaster::SetWindow>();

    const inline static Action UseActionSetWindowBorderless =
        ActionFactory::CreateKeyboardAction<WindowMasterPuppet, &WindowMaster::SetBorderless>();

    const inline static Action UseActionExit =
        ActionFactory::CreateKeyboardAction<WindowMasterPuppet, &WindowMaster::ExitGame>();
};
