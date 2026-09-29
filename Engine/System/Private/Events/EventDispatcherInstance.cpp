#include "B33System.hpp"
#include "Events/EventDispatcherInstance.hpp"

namespace B33::System
{
EventDispatcherInstance::EventDispatcherInstance()
  : m_RegisteredEvents()
  , m_RegisteredEventsArgs()
{
}

void EventDispatcherInstance::HandleNewEvent( EventId eventId )
{
    if ( !m_RegisteredEvents.try_emplace( eventId ).second )
    {
        return;
    }

    m_RegisteredEvents[ eventId ] = FnVector();
}

EventAddr EventDispatcherInstance::Register( EventId eventId, FnPtr pOnEvent )
{
    if ( !m_RegisteredEvents.contains( eventId ) )
    {
        B33_WARNING( L"That event isn't handled by this dispatcher! Returning 0 as EventAddr" );
        return 0;
    }
    m_RegisteredEvents[ eventId ].push_back( pOnEvent );
    return reinterpret_cast<EventAddr>( pOnEvent );
}

void EventDispatcherInstance::Trigger( EventId eventId )
{
    for ( auto *pCall : m_RegisteredEvents[ eventId ] )
    {
        B33_TRACE( L"Dispatching event to %p", pCall );
        pCall();
    }
    B33_TRACE( L"Dispatching event to %d listeners", m_RegisteredEventsArgs[ eventId ].size() );
    for ( auto &call : m_RegisteredEventsArgs[ eventId ] )
    {
        B33_TRACE( L"Dispatching event to %p", call.get() );
        call->OnCall();
    }
    B33_TRACE( L"No more listeners" );
}

void EventDispatcherInstance::UnregisterFunctionHandler( EventId eventId, EventAddr pAddr )
{
    for ( auto it = m_RegisteredEvents[ eventId ].begin(); it != m_RegisteredEvents[ eventId ].end(); ++it )
    {
        if ( reinterpret_cast<EventAddr>( *it ) == pAddr )
        {
            m_RegisteredEvents[ eventId ].erase( it );
            break;
        }
    }
}

void EventDispatcherInstance::UnregisterObjectHandler( EventId eventId, EventAddr pAddr )
{
    for ( auto it = m_RegisteredEventsArgs[ eventId ].begin(); it != m_RegisteredEventsArgs[ eventId ].end(); ++it )
    {
        if ( reinterpret_cast<EventAddr>( it->get() ) == pAddr )
        {
            m_RegisteredEventsArgs[ eventId ].erase( it );
            break;
        }
    }
}

} // namespace B33::System
