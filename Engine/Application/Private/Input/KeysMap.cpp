#include "B33App.h"

#include "KeysMap.hpp"

namespace B33::App
{

// Constructors // ----------------------------------------------------------------------------------------------------
KeysMap::KeysMap()
  : m_vKeys( AmountOfBindableKeys )
{
}

// --------------------------------------------------------------------------------------------------------------------
KeysMap::KeysMap( usize uAmountOfBindableKeys )
  : m_vKeys( uAmountOfBindableKeys )
{
}

// Impl // ------------------------------------------------------------------------------------------------------------
void KeysMap::BindActionImpl( const B33InputBind                            &ib,
                              void                                          *pThis,
                              B33Action                                      a,
                              __B33_ATTRIBUTE_MIGHT_BE_UNUSED B33MouseAction ma )
{
    B33_ASSERT( ib.Type == EB33BindType::Keyboard || ib.Type == EB33BindType::MouseButton );
    B33_ASSERT( ib.Keyboard.KeyCode > B33_INVALID_KEY && ib.Keyboard.KeyCode < B33_KEY_COUNT );
    B33_ASSERT( m_vKeys[ ib.Keyboard.KeyCode ].This == nullptr );
    B33_ASSERT( ma == nullptr );
    B33_ASSERT( pThis != nullptr );

    m_vKeys[ ib.Keyboard.KeyCode ] = ActionReplayData( pThis, a );
}

// ---------------------------------------------------------------------------------------------------------------------
void KeysMap::UnbindActionImpl( const B33InputBind &ib, void __B33_ATTRIBUTE_MIGHT_BE_UNUSED *pThis )
{
    B33_ASSERT( ib.Type == EB33BindType::Keyboard || ib.Type == EB33BindType::MouseButton );
    B33_ASSERT( ib.Keyboard.KeyCode > B33_INVALID_KEY && ib.Keyboard.KeyCode < B33_KEY_COUNT );
    B33_ASSERT( pThis != nullptr );

    m_vKeys[ ib.Keyboard.KeyCode ].This = nullptr;
}

// Public // ----------------------------------------------------------------------------------------------------------
void KeysMap::PlayAction( const float fDelta, B33KeyId keyCode ) noexcept
{
    B33_ASSERT( keyCode > B33_INVALID_KEY && keyCode < B33_KEY_COUNT );

    const auto &playableAction = m_vKeys[ keyCode ];

    if ( playableAction.This )
    {
        playableAction.Action( fDelta, playableAction.This );
    }
}

} // namespace B33::App
