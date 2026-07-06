#include <iostream>
#include <memory>
#include <tuple>
#include <unordered_set>

using namespace std;

// Unknown // ---------------------------------------------------------------------------------------------------------
template <class T>
class Unknown
{
    static inline int index = 0;

  public:
    Unknown();

    static int GetIndex()
    {
        return index;
    }
};

static inline int counter = 0;

int IncreaseAndGetCounter()
{
    return ++counter;
}

template <class T>
Unknown<T>::Unknown()
{
    if ( this->index == 0 )
        index = IncreaseAndGetCounter();
}

// --------------------------------------------------------------------------------------------------------------------

class ICall
{
  public:
    virtual ~ICall() = default;

  public:
    virtual void OnCall() = 0;
};

template <class OBJECT_CLASS>
class Call : public ICall
{
  public:
    Call( OBJECT_CLASS *pObj, void ( OBJECT_CLASS ::*pCall )() )
      : m_pObj( pObj )
      , m_pCall( pCall )
    {
    }

    static Call *CreateCall( OBJECT_CLASS *pObj, void ( OBJECT_CLASS ::*pCall )() )
    {
        return new Call( pObj, pCall );
    }

  public:
    virtual void OnCall() override final
    {
        ( m_pObj->*m_pCall )();
    }

  private:
    OBJECT_CLASS *m_pObj               = nullptr;
    void ( OBJECT_CLASS ::*m_pCall )() = nullptr;
};

typedef uint32_t EventId;
typedef size_t   EventAddr;

class EventDispatcherInstance
{
    using UniqueCall  = unique_ptr<ICall>;
    using FnPtr       = void ( * )();
    using EventSet    = unordered_map<EventId, vector<FnPtr>>;
    using EventSetArg = unordered_map<EventId, vector<UniqueCall>>;

  public:
    EventDispatcherInstance()  = default;
    ~EventDispatcherInstance() = default;

  public:
    EventDispatcherInstance( EventDispatcherInstance && )                 = default;
    EventDispatcherInstance &operator=( EventDispatcherInstance && )      = default;
    EventDispatcherInstance( const EventDispatcherInstance & )            = default;
    EventDispatcherInstance &operator=( const EventDispatcherInstance & ) = default;

  public:
    void HandleNewEvent( EventId eventId )
    {
        if ( m_RegisteredEvents.contains( eventId ) )
        {
            cerr << "That event already exsits!" << endl;
            return;
        }

        m_RegisteredEvents[ eventId ] = {};
    }

    EventAddr Register( EventId eventId, FnPtr pOnEvent )
    {
        if ( !m_RegisteredEvents.contains( eventId ) )
        {
            cerr << "That event isn't handled by this dispatcher!" << endl;
            return 0;
        }
        m_RegisteredEvents[ eventId ].push_back( pOnEvent );
        return reinterpret_cast<EventAddr>( pOnEvent );
    }

    template <class OBJECT_CLASS>
    EventAddr Register( EventId eventId, OBJECT_CLASS *pObj, void ( OBJECT_CLASS ::*pCall )() )
    {
        if ( !m_RegisteredEvents.contains( eventId ) )
        {
            cerr << "That event isn't handled by this dispatcher!" << endl;
            return 0;
        }
        m_RegisteredEventsArgs[ eventId ].push_back(
            unique_ptr<ICall>( Call<OBJECT_CLASS>::CreateCall( pObj, pCall ) ) );
        return reinterpret_cast<EventAddr>( m_RegisteredEventsArgs[ eventId ].back().get() );
    }

    void Trigger( EventId eventId )
    {
        for ( auto *pCall : m_RegisteredEvents[ eventId ] )
        {
            pCall();
        }
        for ( auto &call : m_RegisteredEventsArgs[ eventId ] )
        {
            call->OnCall();
        }
    }

    void UnregisterFunctionHandler( EventId eventId, EventAddr pAddr )
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

    void UnregisterObjectHandler( EventId eventId, EventAddr pAddr )
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

  private:
    EventSet    m_RegisteredEvents     = {};
    EventSetArg m_RegisteredEventsArgs = {};
};

enum EEventHandleType
{
    Invalid,
    Funciton,
    Object,
};

class EventHandle
{
    using FnPtr      = void ( * )();
    using UniqueCall = unique_ptr<ICall>;

  public:
    EventHandle( const shared_ptr<EventDispatcherInstance> pInstance,
                 EventId                                   eventId,
                 EventAddr                                 pAddr,
                 EEventHandleType                          type )
      : m_EventId( eventId )
      , m_pInstance( pInstance )
      , m_pEventAddr( reinterpret_cast<EventAddr>( pAddr ) )
      , m_eType( type )
    {
    }

    ~EventHandle()
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

  public:
    EventHandle( EventHandle && )                 = default;
    EventHandle &operator=( EventHandle && )      = default;
    EventHandle( const EventHandle & )            = default;
    EventHandle &operator=( const EventHandle & ) = default;

  private:
    EventId                           m_EventId    = -1;
    weak_ptr<EventDispatcherInstance> m_pInstance  = {};
    EventAddr                         m_pEventAddr = 0;
    EEventHandleType                  m_eType      = Invalid;
};

template <class EVENT_NAME>
class Event : public Unknown<EVENT_NAME>
{
};

class OnScreenEvent : public Event<OnScreenEvent>
{
};

class OnMouseEvent : public Event<OnMouseEvent>
{
};

class EventDispatcher
{
    using FnPtr = void ( * )();

  public:
    EventDispatcher()
      : m_pInstance( make_shared<EventDispatcherInstance>() )
    {
    }

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
        m_pInstance->HandleNewEvent( EVENT::GetIndex() );
        auto r = m_pInstance->Register( EVENT::GetIndex(), pObj, pMethod );

        return EventHandle( m_pInstance, EVENT::GetIndex(), r, EEventHandleType::Object );
    }

    template <class EVENT>
    EventHandle Register( FnPtr pOnEvent )
    {
        {
            EVENT e = {};
            (void)e;
        }
        m_pInstance->HandleNewEvent( EVENT::GetIndex() );
        auto r = m_pInstance->Register( EVENT::GetIndex(), pOnEvent );

        return EventHandle( m_pInstance, EVENT::GetIndex(), r, EEventHandleType::Funciton );
    }

    template <class EVENT>
    void Trigger()
    {
        m_pInstance->Trigger( EVENT::GetIndex() );
    }

  private:
    shared_ptr<EventDispatcherInstance> m_pInstance = nullptr;
};

class Dog
{
  public:
    void Bark()
    {
        cout << "Dog dog" << endl;
    }
};

int main()
{
    EventDispatcher dispatcher = {};
    Dog             doggie     = {};
    {
        auto handle1 = dispatcher.Register<OnScreenEvent>( +[]()
                                                           {
                                                               cout << "HIT" << endl;
                                                           } );

        auto handle2 = dispatcher.Register<OnScreenEvent>( +[]()
                                                           {
                                                               cout << "AnotherHIT" << endl;
                                                           } );

        auto handle3 = dispatcher.Register<OnMouseEvent>( +[]()
                                                          {
                                                              cout << "MOUSE" << endl;
                                                          } );

        auto handle4 = dispatcher.Register<OnMouseEvent>( &doggie, &Dog::Bark );
    }
    auto handle1 = dispatcher.Register<OnScreenEvent>( +[]()
                                                       {
                                                           cout << "HIT" << endl;
                                                       } );

    auto handle2 = dispatcher.Register<OnScreenEvent>( +[]()
                                                       {
                                                           cout << "AnotherHIT" << endl;
                                                       } );

    auto handle3 = dispatcher.Register<OnMouseEvent>( +[]()
                                                      {
                                                          cout << "MOUSE" << endl;
                                                      } );

    auto handle4 = dispatcher.Register<OnMouseEvent>( &doggie, &Dog::Bark );
    auto handle5 = dispatcher.Register<OnMouseEvent>( +[]()
                                                      {
                                                          cout << "MOUSE" << endl;
                                                      } );
    for ( int i = 0; i < 10; ++i )
    {
        cout << i + 1 << endl;
        dispatcher.Trigger<OnScreenEvent>();
        dispatcher.Trigger<OnMouseEvent>();
    }
}
