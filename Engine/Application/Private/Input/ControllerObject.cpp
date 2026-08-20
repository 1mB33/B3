#include "B33App.h"

#include "Input/ControllerObject.hpp"

namespace B33::App
{

// Contructors // -----------------------------------------------------------------------------------------------------
ControllerObject::~ControllerObject() noexcept
{
    if ( auto pUserInput = m_pUserInput.lock() )
        pUserInput->Unbind( this );
}

// --------------------------------------------------------------------------------------------------------------------
ControllerObject::ControllerObject( ControllerObject &&other ) noexcept
  : m_pUserInput( std::move( other.m_pUserInput ) )
{
}

// Public // ----------------------------------------------------------------------------------------------------------
void ControllerObject::SignObject( ::std::weak_ptr<UserInput> pUserInput )
{
    // We can be signed by only one UserInput
    if ( !m_pUserInput.expired() )
    {
        B33_ASSERT( m_pUserInput.lock().get() == pUserInput.lock().get() );
        B33_LOG( Core::Debug::Warning, L"ControllerObject can be signed only by one UserInput." );
        return;
    }

    m_pUserInput = pUserInput;
}

} // namespace B33::App
