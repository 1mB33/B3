#if !defined( B33_ICOMPONENT_H )
#    define B33_ICOMPONENT_H

#    include <B33Core.h>
#    include "Synchronization/DeltaTime.hpp"

namespace B33::System
{

enum EComponentType
{
    Async           = 1,
    Default         = Async << 1,
    AsyncUpdateOnly = Default << 1,
};

class ComponentAbstractBase;
using ComponentInstance = ::std::unique_ptr<ComponentAbstractBase>;
using ComponentFactory  = ComponentInstance ( * )();

class ComponentAbstractBase
{
    friend class EngineLoop;
    friend class ComponentBridge;

    using Ai32         = ::std::atomic_int32_t;
    using ABool        = ::std::atomic_bool;
    using Mutex        = ::std::mutex;
    using ConditionVar = ::std::condition_variable;

  public:
    virtual EComponentType GetComponentType() = 0;

  public:
    ComponentAbstractBase()
      : m_UpdateCount( 0 )
      , m_mUsed()
      , m_bFree( true )
      , m_Conditional()
      , m_LocalDelta()
    {
    }

    virtual ~ComponentAbstractBase() noexcept = default;

  public:
    float GetLocalDelta()
    {
        return m_LocalDelta.FetchMs();
    }

  public:
    void SetDeltaRefrenceFrame()
    {
        m_LocalDelta.SetReferenceFrame();
    }

  public:
    virtual void Initialize( class ComponentBridge &bridge )           = 0;
    virtual void Update( float fDelta, class ComponentBridge &bridge ) = 0;
    virtual void Destroy( class ComponentBridge &bridge )              = 0;

    void Lock()
    {
        m_mUsed.lock();
        B33_TRACE( L"Component locked %p", this );
    }

    void Unlock()
    {
        m_mUsed.unlock();
        B33_TRACE( L"Component unlocked %p", this );
    }

  private:
    void IncreaseCount()
    {
        m_UpdateCount.store( m_UpdateCount.load() + 1 );
    }

    void DecreaseCount()
    {
        m_UpdateCount.store( m_UpdateCount.load() - 1 );
    }

    i32 GetCount()
    {
        return m_UpdateCount.load();
    }

  private:
    Ai32            m_UpdateCount;
    Mutex           m_mUsed;
    ABool           m_bFree;
    ConditionVar    m_Conditional;
    Core::DeltaTime m_LocalDelta;
};

class Component : public ComponentAbstractBase
{
  public:
    virtual EComponentType GetComponentType() override final
    {
        return EComponentType::Default;
    }
};

class ComponentAsync : public ComponentAbstractBase
{
  public:
    virtual EComponentType GetComponentType() override
    {
        return EComponentType::Async;
    }
};

class ComponentAsyncUpdateOnly : public ComponentAsync
{
  public:
    virtual EComponentType GetComponentType() override final
    {
        return EComponentType::AsyncUpdateOnly;
    }
};

class ComponentInstanceRegister
{
    using StringView = ::std::string_view;

  private:
    ComponentInstanceRegister() = default;

  public:
    ~ComponentInstanceRegister() noexcept = default;

    ComponentInstanceRegister( ComponentInstanceRegister && )      = default;
    ComponentInstanceRegister( const ComponentInstanceRegister & ) = default;

    ComponentInstanceRegister &operator=( ComponentInstanceRegister && )      = default;
    ComponentInstanceRegister &operator=( const ComponentInstanceRegister & ) = default;

  public:
    template <class COMPONENT_DERIVED>
    static constexpr ComponentInstanceRegister Register( const StringView &className )
    {
        return Register( className, &COMPONENT_DERIVED::GetComponentFactory );
    }

    static ComponentInstanceRegister Register( const StringView &className, ComponentFactory factory )
    {
        RegisterInternal( className, factory );
        return {};
    }

  private:
    __B33_API static void RegisterInternal( const StringView &className, ComponentFactory factory );
};

#    define B33_COMPONENT( CLASS_NAME )                                                                                \
      public:                                                                                                          \
        static ::B33::System::ComponentInstance GetComponentFactory()                                                  \
        {                                                                                                              \
            return ::std::make_unique<CLASS_NAME>();                                                                   \
        }                                                                                                              \
        static ::std::string_view GetComponentName()                                                                   \
        {                                                                                                              \
            return #CLASS_NAME;                                                                                        \
        }                                                                                                              \
                                                                                                                       \
      private:                                                                                                         \
        static inline const ::B33::System::ComponentInstanceRegister RegisteredComponent =                             \
            ::B33::System::ComponentInstanceRegister::Register( #CLASS_NAME, &GetComponentFactory );


} // namespace B33::System
#endif // !B33_ICOMPONENT_H
