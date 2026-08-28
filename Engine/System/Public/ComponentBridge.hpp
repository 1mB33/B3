#ifndef B33_COMPONENT_BRIDGE_H
#define B33_COMPONENT_BRIDGE_H

#include "IComponent.hpp"

namespace B33::System
{

template <class COMPONENT>
class BorrowedComponent
{
  public:
    BorrowedComponent( COMPONENT &component )
      : m_Component( component )
    {
        m_Component.Lock();
    }

    ~BorrowedComponent()
    {
        m_Component.Unlock();
    }

  public:
    BorrowedComponent( BorrowedComponent<COMPONENT> && )            = default;
    BorrowedComponent &operator=( BorrowedComponent<COMPONENT> && ) = default;

    BorrowedComponent( const BorrowedComponent<COMPONENT> & )            = delete;
    BorrowedComponent &operator=( const BorrowedComponent<COMPONENT> & ) = delete;

  public:
    COMPONENT *operator->()
    {
        return &m_Component;
    }

  private:
    COMPONENT &m_Component;
};

class ComponentBridge
{
    friend class EngineLoop;

  public:
    ComponentBridge()
      : m_ComponentMap()
    {
    }

    ~ComponentBridge() = default;

    ComponentBridge( const ComponentBridge & ) = delete;
    ComponentBridge( ComponentBridge && )      = delete;

    ComponentBridge &operator=( const ComponentBridge & ) = delete;
    ComponentBridge &operator=( ComponentBridge && )      = delete;

  public:
    __B33_API ComponentAbstractBase &QueryComponent( ::std::string strComponentName )
    {
        B33_ASSERT( m_ComponentMap.find( strComponentName ) != m_ComponentMap.end() );
        return *m_ComponentMap[ strComponentName ].get();
    }

    ComponentAbstractBase *QueryComponentByClassName( ::std::string_view ComponentDerived )
    {
        B33_ASSERT( m_ComponentMap.find( ComponentDerived ) != m_ComponentMap.end() );
        ComponentAbstractBase *result = m_ComponentMap[ ComponentDerived ].get();
        return result;
    }

    template <class COMPONENT_DERIVED>
    BorrowedComponent<COMPONENT_DERIVED> QueryComponent()
    {
        B33_ASSERT( m_ComponentMap.find( COMPONENT_DERIVED::GetComponentName() ) != m_ComponentMap.end() );
        ComponentAbstractBase *result        = m_ComponentMap[ COMPONENT_DERIVED::GetComponentName() ].get();
        COMPONENT_DERIVED     *resultDerived = dynamic_cast<COMPONENT_DERIVED *>( result );
        return BorrowedComponent<COMPONENT_DERIVED>( *resultDerived );
    }

  private:
    ::std::unordered_map<::std::string_view, ::std::unique_ptr<ComponentAbstractBase>> m_ComponentMap = {};
};

} // namespace B33::System
#endif // !B33_COMPONENT_BRIDGE_H
