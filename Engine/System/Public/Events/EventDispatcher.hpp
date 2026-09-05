#if !defined( B33_EVENT_DISPATCHER_HPP )
#    define B33_EVENT_DISPATCHER_HPP

#    include "EventHandle.hpp"
#    include "EventDispatcherInstance.hpp"

namespace B33::System
{

class EventDispatcher
{
    using FnPtr                   = void ( * )();
    using SharedDispacherInstance = ::std::shared_ptr<EventDispatcherInstance>;

    template <typename T>
    static constexpr decltype( auto ) MakeShared()
    {
        return ::std::make_shared<T>();
    }

    template <typename T, typename U>
    static constexpr decltype( auto ) MakeShared( U &&arg )
    {
        return ::std::make_shared<T>( Forward<U>( arg ) );
    }

  public:
    __B33_API EventDispatcher();

    ~EventDispatcher() noexcept = default;

  public:
    EventDispatcher &operator=( EventDispatcher && )      = default;
    EventDispatcher( EventDispatcher && )                 = default;
    EventDispatcher &operator=( const EventDispatcher & ) = default;
    EventDispatcher( const EventDispatcher & )            = default;

  public:
    template <class EVENT, class OBJECT_CLASS_PTR, class OBJECTS_METHOD_PTR>
    __B33_ATTRIBUTE_WARN_UNUSED EventHandle Register( OBJECT_CLASS_PTR pObj, OBJECTS_METHOD_PTR pMethod )
    {
        EVENT::template Invoke<EVENT>();

        B33_TRACE( L"Registering event id %d for %p with %p", EVENT::GetGlobalIndex(), pObj, pMethod );

        m_pInstance->HandleNewEvent( EVENT::GetGlobalIndex() );
        auto r = m_pInstance->Register( EVENT::GetGlobalIndex(), pObj, pMethod );

        return EventHandle( m_pInstance, EVENT::GetGlobalIndex(), r, EEventHandleType::Object );
    }

    template <class EVENT>
    EventHandle Register( FnPtr pOnEvent )
    {
        EVENT::template Invoke<EVENT>();

        m_pInstance->HandleNewEvent( EVENT::GetGlobalIndex() );
        auto r = m_pInstance->Register( EVENT::GetGlobalIndex(), pOnEvent );

        return EventHandle( m_pInstance, EVENT::GetGlobalIndex(), r, EEventHandleType::Funciton );
    }

    template <class EVENT>
    void Trigger()
    {
        B33_TRACE( L"Triggering event id: %d", EVENT::GetGlobalIndex() );
        m_pInstance->Trigger( EVENT::GetGlobalIndex() );
    }

  private:
    SharedDispacherInstance m_pInstance = nullptr;
};

} // namespace B33::System
#endif
