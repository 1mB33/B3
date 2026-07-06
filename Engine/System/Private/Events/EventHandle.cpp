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
  , m_pEventAddr( reinterpret_cast<EventAddr>( pAddr ) )
  , m_eType( type )
{
}

EventHandle::~EventHandle()
{
    if ( auto pLock = m_pInstance.lock() )
    {
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
