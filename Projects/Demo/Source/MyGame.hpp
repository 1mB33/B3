#pragma once

#include "MainCharacter.hpp"
#include "B33System.hpp"

class MyGame : public ::B33::System::ComponentAsyncUpdateOnly
{
    B33_COMPONENT( MyGame );

  public:
    MyGame();

  public:
    virtual void Initialize( ::B33::System::ComponentBridge &bridge ) override;
    virtual void Update( float fDelta, ::B33::System::ComponentBridge &bridge ) override;
    virtual void Destroy( ::B33::System::ComponentBridge &bridge ) override;

  public:
    const Game &GetGameInstance() const
    {
        return m_Game;
    }

    Game &GetGameInstance()
    {
        return m_Game;
    }

    const PlayablePaper &GetMainCharacter() const;

  private:
    Game          m_Game = {};
    PlayablePaper m_Paper;
};
