#include "B33App.h"

#include "Input/ControllerObject.hpp"

namespace B33::App
{

// Contructors // -----------------------------------------------------------------------------------------------------
ControllerObject::ControllerObject() noexcept
  : m_pUserInput()
{
}

// --------------------------------------------------------------------------------------------------------------------
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

// --------------------------------------------------------------------------------------------------------------------
ControllerObject &ControllerObject::operator=( ControllerObject &&other ) noexcept
{
    m_pUserInput = other.m_pUserInput;

    other.UnsignObject();

    return *this;
}

// Public // ----------------------------------------------------------------------------------------------------------
void ControllerObject::SignObject( WeakPtr<UserInput> pUserInput ) noexcept
{
    // We can be signed by only one UserInput
    if ( !m_pUserInput.expired() )
    {
        B33_ASSERT( m_pUserInput.lock().get() == pUserInput.lock().get() );
        B33_WARNING( L"ControllerObject can be signed only by one UserInput." );
        return;
    }

    m_pUserInput = pUserInput;
}

// --------------------------------------------------------------------------------------------------------------------
void ControllerObject::UnsignObject() noexcept
{
    if ( !m_pUserInput.expired() )
    {
        B33_WARNING( L"Calling unsign on unsigned object" );
        return;
    }

    m_pUserInput = WeakPtr<UserInput>();
}

} // namespace B33::App
