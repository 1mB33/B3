#pragma once

#include "B33System.hpp"
#include "ComponentsOrder.hpp"
#include "EmptyCanvas.hpp"
#include "IComponent.hpp"
#include "WindowMaster.hpp"
#include "Window/WindowPolicy/GameSystemPolicy.hpp"

B33_CREATE_COMPONENTS( "MainWindow", "MyGame", "Renderer" )

class MainWindow : public ::B33::System::IComponentNoBridge
{
    B33_COMPONENT( MainWindow );

  public:
    MainWindow()
      : m_WindowInstance( L"Cool Game", 1200, 700 )
      , m_WindowPuppet( m_WindowInstance )
    {
    }

  public:
    virtual void Initialize() override;
    virtual void Update( float fDelta ) override;
    virtual void Destroy() override;

  public:
    const ::B33::App::EmptyCanvas<true, ::B33::App::DefaultGameSystemWindowPolicy> &GetWindowInstance()
    {
        return m_WindowInstance;
    }

  private:
    ::B33::App::EmptyCanvas<true, ::B33::App::DefaultGameSystemWindowPolicy> m_WindowInstance;
    WindowMasterPuppet                                                       m_WindowPuppet;
};
