#pragma once

#include "MainCharacter.hpp"
#include "B33System.hpp"

class MyGame : public ::B33::System::IComponentAsync
{
    B33_COMPONENT( MyGame );

  public:
    MyGame()
      : m_Game()
      , m_Paper( m_Game )
    {
    }

  public:
    virtual void Initialize( ::B33::System::ComponentBridge &bridge ) override;
    virtual void Update( float fDelta ) override;
    virtual void Destroy( ::B33::System::ComponentBridge &bridge ) override;

  public:
    const Game &GetGameInstance() const
    {
        return m_Game;
    }

    const PlayablePaper &GetMainCharacter() const
    {
        return m_Paper;
    }

  private:
    Game          m_Game = {};
    PlayablePaper m_Paper;
};
