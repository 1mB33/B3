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

class EventDispatcherInstance
{
    using UniqueCall  = unique_ptr<ICall>;
    using EventSet    = unordered_map<int, vector<void ( * )()>>;
    using EventSetArg = unordered_map<int, vector<UniqueCall>>;

  public:
    EventDispatcherInstance()  = default;
    ~EventDispatcherInstance() = default;

  public:
    EventDispatcherInstance( EventDispatcherInstance && )                 = default;
    EventDispatcherInstance &operator=( EventDispatcherInstance && )      = default;
    EventDispatcherInstance( const EventDispatcherInstance & )            = default;
    EventDispatcherInstance &operator=( const EventDispatcherInstance & ) = default;

  public:
    void HandleNewEvent( int eventId )
    {
        if ( m_RegisteredEvents.contains( eventId ) )
        {
            cerr << "That event already exsits!" << endl;
            return;
        }

        m_RegisteredEvents[ eventId ] = {};
    }

    void *Register( int eventId, void ( *pOnEvent )() )
    {
        if ( !m_RegisteredEvents.contains( eventId ) )
        {
            cerr << "That event isn't handled by this dispatcher!" << endl;
            return nullptr;
        }
        m_RegisteredEvents[ eventId ].push_back( pOnEvent );
        return reinterpret_cast<void *>( pOnEvent );
    }

    template <class OBJECT_CLASS>
    void *Register( int eventId, OBJECT_CLASS *pObj, void ( OBJECT_CLASS ::*pCall )() )
    {
        if ( !m_RegisteredEvents.contains( eventId ) )
        {
            cerr << "That event isn't handled by this dispatcher!" << endl;
            return nullptr;
        }
        m_RegisteredEventsArgs[ eventId ].push_back(
            unique_ptr<ICall>( Call<OBJECT_CLASS>::CreateCall( pObj, pCall ) ) );
        return m_RegisteredEventsArgs[ eventId ].back().get();
    }

    void Trigger( int eventId )
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

    void UnregisterHandler( int eventId, void *pAddr )
    {
        for ( auto it = m_RegisteredEvents[ eventId ].begin(); it != m_RegisteredEvents[ eventId ].end(); ++it )
        {
            if ( reinterpret_cast<void *>( *it ) == pAddr )
            {
                m_RegisteredEvents[ eventId ].erase( it );
                break;
            }
        }
        for ( auto it = m_RegisteredEventsArgs[ eventId ].begin(); it != m_RegisteredEventsArgs[ eventId ].end(); ++it )
        {
            if ( reinterpret_cast<void *>( it->get() ) == pAddr )
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

class EventHandle
{
    using UniqueCall = unique_ptr<ICall>;

  public:
    EventHandle( const shared_ptr<EventDispatcherInstance> pInstance, int eventId, void *pFnAddr )
      : m_EventId( eventId )
      , m_pInstance( pInstance )
      , m_pAddrCall( nullptr )
      , m_pFnAddr( pFnAddr )
    {
    }

    EventHandle( const shared_ptr<EventDispatcherInstance> pInstance, int eventId, UniqueCall *pCallAddr )
      : m_EventId( eventId )
      , m_pInstance( pInstance )
      , m_pAddrCall( pCallAddr )
      , m_pFnAddr( nullptr )
    {
    }

    ~EventHandle()
    {
        if ( auto pLock = m_pInstance.lock() )
        {
            if ( m_pAddrCall )
            {
                pLock->UnregisterHandler( m_EventId, m_pAddrCall );
            }
            if ( m_pFnAddr )
            {
                pLock->UnregisterHandler( m_EventId, m_pFnAddr );
            }
        }
    }

  public:
    EventHandle( EventHandle && )                 = default;
    EventHandle &operator=( EventHandle && )      = default;
    EventHandle( const EventHandle & )            = default;
    EventHandle &operator=( const EventHandle & ) = default;

  private:
    int                               m_EventId   = -1;
    weak_ptr<EventDispatcherInstance> m_pInstance = {};
    UniqueCall                       *m_pAddrCall = nullptr;
    void                             *m_pFnAddr   = nullptr;
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
    template <class EVENT, class OBJECT_CLASS>
    EventHandle Register( OBJECT_CLASS *pObj, void ( OBJECT_CLASS ::*pCall )() )
    {
        EVENT e = {};
        (void)e;
        m_pInstance->HandleNewEvent( EVENT::GetIndex() );
        auto r = m_pInstance->Register( EVENT::GetIndex(), pObj, pCall );

        return EventHandle( m_pInstance, EVENT::GetIndex(), r );
    }

    template <class EVENT>
    EventHandle Register( void ( *pOnEvent )() )
    {
        EVENT e = {};
        (void)e;
        m_pInstance->HandleNewEvent( EVENT::GetIndex() );
        auto r = m_pInstance->Register( EVENT::GetIndex(), pOnEvent );

        return EventHandle( m_pInstance, EVENT::GetIndex(), r );
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
