#if !defined( B33_EVENT_DISPATCHER_INSTANCE_HPP )
#    define B33_EVENT_DISPATCHER_INSTANCE_HPP

#    include "B33System.hpp"
#    include "Call.hpp"
#    include "EventType.hpp"

namespace B33::System
{

class EventDispatcherInstance
{
    using UniqueICall          = ::std::unique_ptr<ICall>;
    using FnPtr                = void ( * )();
    using FnVector             = ::std::vector<FnPtr>;
    using UniqueCallVector     = ::std::vector<UniqueICall>;
    using EventSet             = ::std::unordered_map<EventId, FnVector>;
    using EventSetObjectMethod = ::std::unordered_map<EventId, UniqueCallVector>;

  public:
    __B33_API EventDispatcherInstance();
    ~EventDispatcherInstance() = default;

  public:
    EventDispatcherInstance( EventDispatcherInstance && )                 = default;
    EventDispatcherInstance &operator=( EventDispatcherInstance && )      = default;
    EventDispatcherInstance( const EventDispatcherInstance & )            = default;
    EventDispatcherInstance &operator=( const EventDispatcherInstance & ) = default;

  public:
    __B33_API void HandleNewEvent( EventId eventId );

    __B33_API EventAddr Register( EventId eventId, FnPtr pOnEvent );

    template <class OBJECT_CLASS>
    EventAddr Register( EventId eventId, OBJECT_CLASS *pObj, void ( OBJECT_CLASS ::*pCall )() )
    {
        if ( !m_RegisteredEvents.contains( eventId ) )
        {
            B33_WARNING( L"That event isn't handled by this dispatcher! Returning 0 as EventAddr" );
            return 0;
        }
        m_RegisteredEventsArgs[ eventId ].push_back( UniqueICall( Call<OBJECT_CLASS>::CreateCall( pObj, pCall ) ) );

        return reinterpret_cast<EventAddr>( m_RegisteredEventsArgs[ eventId ].back().get() );
    }

    __B33_API void Trigger( EventId eventId );

    __B33_API void UnregisterFunctionHandler( EventId eventId, EventAddr pAddr );

    __B33_API void UnregisterObjectHandler( EventId eventId, EventAddr pAddr );

  private:
    EventSet             m_RegisteredEvents     = {};
    EventSetObjectMethod m_RegisteredEventsArgs = {};
};

} // namespace B33::System

#endif
