#pragma once

#include "B33System.hpp"
#include "B33Rendering.hpp"
#include "Input/ControllerObject.hpp"
#include "Vulkan/Renderer.hpp"
#include "Input/Action.hpp"
#include "Input/Playable.hpp"

class RendererMaster
{
  public:
    explicit RendererMaster( ::B33::Rendering::Renderer &renderer )
      : m_RendererInstance( renderer )
      , m_bDebugMode( false )
    {
    }

    ~RendererMaster() = default;

  public:
    bool GetDebugMode() const
    {
        return m_bDebugMode;
    }

  public:
    void SwitchDebugMode( float )
    {
        m_bDebugMode = !m_bDebugMode;
    }

  private:
    ::B33::Rendering::Renderer &m_RendererInstance;
    bool                        m_bDebugMode = false;
};

class RendererMasterController : public ::B33::App::ControllerObject
{
    using Action        = ::B33::App::Action;
    using ActionFactory = ::B33::App::ActionFactory;

  public:
    const inline static Action UseActionSwitchDebugMode =
        ActionFactory::CreateKeyboardAction<RendererMaster, &RendererMaster::SwitchDebugMode>();
};

typedef ::B33::App::Playable<RendererMaster, RendererMasterController> RendererMasterPuppet;
