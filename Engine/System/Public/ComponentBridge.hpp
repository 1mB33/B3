#ifndef B33_COMPONENT_BRIDGE_H
#define B33_COMPONENT_BRIDGE_H

#include "IComponent.hpp"

namespace B33::System
{

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
    __B33_API IComponentAbstractBase &QueryComponent( ::std::string strComponentName )
    {
        B33_ASSERT( m_ComponentMap.find( strComponentName ) != m_ComponentMap.end() );
        return *m_ComponentMap[ strComponentName ].get();
    }

    IComponentAbstractBase *QueryComponentByClassName( ::std::string_view ComponentDerived )
    {
        B33_ASSERT( m_ComponentMap.find( ComponentDerived ) != m_ComponentMap.end() );
        IComponentAbstractBase *result        = m_ComponentMap[ ComponentDerived ].get();
        IComponentAbstractBase *resultDerived = result;
        return resultDerived;
    }

    template <class COMPONENT_DERIVED>
    COMPONENT_DERIVED &QueryComponent()
    {
        B33_ASSERT( m_ComponentMap.find( COMPONENT_DERIVED::GetComponentName() ) != m_ComponentMap.end() );
        IComponentAbstractBase *result        = m_ComponentMap[ COMPONENT_DERIVED::GetComponentName() ].get();
        COMPONENT_DERIVED      *resultDerived = dynamic_cast<COMPONENT_DERIVED *>( result );
        return *resultDerived;
    }

  private:
    ::std::unordered_map<::std::string_view, ::std::unique_ptr<IComponentAbstractBase>> m_ComponentMap = {};
};

} // namespace B33::System
#endif // !B33_COMPONENT_BRIDGE_H
