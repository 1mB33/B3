#if !defined( B33_EVENT_DISPATCHER_HPP )
#    define B33_EVENT_DISPATCHER_HPP

#    include "B33System.hpp"
#    include "EventHandle.hpp"
#    include "EventDispatcherInstance.hpp"

namespace B33::System
{

class EventDispatcher
{
    using FnPtr                   = void ( * )();
    using SharedDispacherInstance = ::std::shared_ptr<EventDispatcherInstance>;

  public:
    __B33_API EventDispatcher();

    ~EventDispatcher() = default;

  public:
    EventDispatcher &operator=( EventDispatcher && )      = default;
    EventDispatcher( EventDispatcher && )                 = default;
    EventDispatcher &operator=( const EventDispatcher & ) = default;
    EventDispatcher( const EventDispatcher & )            = default;

  public:
    template <class EVENT, class OBJECT_CLASS_PTR, class OBJECTS_METHOD_PTR>
    EventHandle Register( OBJECT_CLASS_PTR pObj, OBJECTS_METHOD_PTR pMethod )
    {
        {
            EVENT e = {};
            (void)e;
        }
        m_pInstance->HandleNewEvent( EVENT::GetGlobalIndex() );
        auto r = m_pInstance->Register( EVENT::GetGlobalIndex(), pObj, pMethod );

        return EventHandle( m_pInstance, EVENT::GetGlobalIndex(), r, EEventHandleType::Object );
    }

    template <class EVENT>
    EventHandle Register( FnPtr pOnEvent )
    {
        {
            EVENT e = {};
            (void)e;
        }
        m_pInstance->HandleNewEvent( EVENT::GetGlobalIndex() );
        auto r = m_pInstance->Register( EVENT::GetGlobalIndex(), pOnEvent );

        return EventHandle( m_pInstance, EVENT::GetGlobalIndex(), r, EEventHandleType::Funciton );
    }

    template <class EVENT>
    void Trigger()
    {
        m_pInstance->Trigger( EVENT::GetGlobalIndex() );
    }

  private:
    SharedDispacherInstance m_pInstance = nullptr;
};

} // namespace B33::System

#endif
