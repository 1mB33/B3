#ifndef B33_ICOMPONENT_H
#define B33_ICOMPONENT_H

#include "B33System.hpp"

namespace B33::System
{

enum EComponentType
{
    AsyncNoBridge = 1,
    Async         = AsyncNoBridge + 1,
    NoBridge      = Async + 1,
    Default       = NoBridge + 1,
};

class IComponentAbstractBase;
using ComponentInstance = ::std::unique_ptr<IComponentAbstractBase>;
using ComponentFactory  = ComponentInstance ( * )();

class IComponentAbstractBase
{
  public:
    virtual ::B33::System::EComponentType GetComponentType() = 0;

  public:
    virtual ~IComponentAbstractBase() = default;

  public:
    virtual void Initialize( class ComponentBridge &bridge ) = 0;
    virtual void Destroy( class ComponentBridge &bridge )    = 0;
};

class IComponentDefault : public IComponentAbstractBase
{
  public:
    virtual ::B33::System::EComponentType GetComponentType() override
    {
        return ::B33::System::EComponentType::Default;
    }

  public:
    virtual void Update( float fDelta, class ComponentBridge &bridge ) = 0;
};

class IComponentAsync : public IComponentAbstractBase
{
  public:
    virtual ::B33::System::EComponentType GetComponentType() override
    {
        return ::B33::System::EComponentType::Async;
    }

  public:
    virtual void Update( float fDelta ) = 0;
};

class IComponentNoBridge : public IComponentAbstractBase
{
  public:
    virtual ::B33::System::EComponentType GetComponentType() override
    {
        return ::B33::System::EComponentType::NoBridge;
    }

  public:
    virtual void Initialize( class ComponentBridge & ) override {}

    virtual void Destroy( class ComponentBridge & ) override {}

    virtual void Initialize()           = 0;
    virtual void Update( float fDelta ) = 0;
    virtual void Destroy()              = 0;
};

class IComponentAsyncNoBridge : public IComponentNoBridge
{
  public:
    virtual ::B33::System::EComponentType GetComponentType() override
    {
        return ::B33::System::EComponentType::AsyncNoBridge;
    }
};

struct ComponentInstanceRegister
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
