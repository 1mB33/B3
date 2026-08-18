#if !defined( B33_EVENT_HANDLE_HPP )
#    define B33_EVENT_HANDLE_HPP

#    include "B33System.hpp"
#    include "EventDispatcherInstance.hpp"

namespace B33::System
{

class EventHandle
{
    using FnPtr                   = void ( * )();
    using UniqueCall              = ::std::unique_ptr<ICall>;
    using SharedDispacherInstance = ::std::shared_ptr<EventDispatcherInstance>;
    using WeakDispacherInstance   = ::std::weak_ptr<EventDispatcherInstance>;

  public:
    __B33_API EventHandle( const SharedDispacherInstance pInstance = nullptr,
                           EventId                       eventId   = -1,
                           EventAddr                     pAddr     = 0,
                           EEventHandleType              type      = Invalid );

    __B33_API ~EventHandle();

  public:
    EventHandle( EventHandle && )                 = default;
    EventHandle &operator=( EventHandle && )      = default;
    EventHandle( const EventHandle & )            = default;
    EventHandle &operator=( const EventHandle & ) = default;

  private:
    EventId               m_EventId    = -1;
    WeakDispacherInstance m_pInstance  = {};
    EventAddr             m_pEventAddr = 0;
    EEventHandleType      m_eType      = Invalid;
};

} // namespace B33::System

#endif
