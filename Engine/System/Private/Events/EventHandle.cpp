#include "B33Core.h"
#include "B33System.hpp"
#include "Events/EventHandle.hpp"

namespace B33::System
{

EventHandle::EventHandle( const EventHandle::SharedDispacherInstance pInstance,
                          EventId                                    eventId,
                          EventAddr                                  pAddr,
                          EEventHandleType                           type )
  : m_EventId( eventId )
  , m_pInstance( pInstance )
  , m_pEventAddr( pAddr )
  , m_eType( type )
{
}

EventHandle::~EventHandle()
{
    if ( auto pLock = m_pInstance.lock() )
    {
        B33_WARNING( L"Event handle is being unregistered since the handle is no longer alive" );

        if ( m_eType == EEventHandleType::Funciton )
        {
            pLock->UnregisterFunctionHandler( m_EventId, m_pEventAddr );
        }
        if ( m_eType == EEventHandleType::Object )
        {
            pLock->UnregisterObjectHandler( m_EventId, m_pEventAddr );
        }
    }
}

} // namespace B33::System
