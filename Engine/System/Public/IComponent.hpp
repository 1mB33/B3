#ifndef B33_ICOMPONENT_H
#define B33_ICOMPONENT_H

#include "B33Core.h"
#include "B33System.hpp"

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

    template <class T>
    friend class BorrowedComponent;

  public:
    virtual ::B33::System::EComponentType GetComponentType() = 0;

  public:
    ComponentAbstractBase()
      : m_UpdateCount( 0 )
      , m_mUsed()
      , m_bFree( true )
      , m_Conditional()
    {
    }

    virtual ~ComponentAbstractBase() = default;

  public:
    virtual void Initialize( class ComponentBridge &bridge )           = 0;
    virtual void Update( float fDelta, class ComponentBridge &bridge ) = 0;
    virtual void Destroy( class ComponentBridge &bridge )              = 0;

  public:
    void Lock()
    {
        m_mUsed.lock();
        B33_TRACE( L"Component locked %p", this );
    }

    bool TryLock()
    {
        return true;
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

    int32_t GetCount()
    {
        return m_UpdateCount.load();
    }

  private:
    ::std::atomic_int32_t     m_UpdateCount;
    ::std::mutex              m_mUsed;
    ::std::atomic_bool        m_bFree;
    ::std::condition_variable m_Conditional;
};

class Component : public ComponentAbstractBase
{
  public:
    virtual ::B33::System::EComponentType GetComponentType() override final
    {
        return ::B33::System::EComponentType::Default;
    }
};

class ComponentAsync : public ComponentAbstractBase
{
  public:
    virtual ::B33::System::EComponentType GetComponentType() override
    {
        return ::B33::System::EComponentType::Async;
    }
};

class ComponentAsyncUpdateOnly : public ComponentAsync
{
  public:
    virtual ::B33::System::EComponentType GetComponentType() override final
    {
        return ::B33::System::EComponentType::AsyncUpdateOnly;
    }
};

class ComponentInstanceRegister
{
  private:
    ComponentInstanceRegister() = default;

  public:
    ~ComponentInstanceRegister() = default;

    ComponentInstanceRegister( ComponentInstanceRegister && )      = default;
    ComponentInstanceRegister( const ComponentInstanceRegister & ) = default;

    ComponentInstanceRegister &operator=( ComponentInstanceRegister && )      = default;
    ComponentInstanceRegister &operator=( const ComponentInstanceRegister & ) = default;

  public:
    template <class COMPONENT_DERIVED>
    static constexpr ComponentInstanceRegister Register( const ::std::string_view &className )
    {
        return Register( className, &COMPONENT_DERIVED::GetComponentFactory );
    }

    static ComponentInstanceRegister Register( const ::std::string_view &className, ComponentFactory factory )
    {
        RegisterInternal( className, factory );
        return {};
    }

  private:
    __B33_API static void RegisterInternal( const ::std::string_view &className, ComponentFactory factory );
};

#define B33_COMPONENT( CLASS_NAME )                                                                                    \
  public:                                                                                                              \
    static ::B33::System::ComponentInstance GetComponentFactory()                                                      \
    {                                                                                                                  \
        return ::std::make_unique<CLASS_NAME>();                                                                       \
    }                                                                                                                  \
    static ::std::string_view GetComponentName()                                                                       \
    {                                                                                                                  \
        return #CLASS_NAME;                                                                                            \
    }                                                                                                                  \
                                                                                                                       \
  private:                                                                                                             \
    static inline const ::B33::System::ComponentInstanceRegister RegisteredComponent =                                 \
        ::B33::System::ComponentInstanceRegister::Register( #CLASS_NAME, &GetComponentFactory );


} // namespace B33::System
#endif // !B33_ICOMPONENT_H
