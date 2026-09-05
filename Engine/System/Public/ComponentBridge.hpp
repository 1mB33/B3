#if !defined( B33_COMPONENT_BRIDGE_HPP )
#    define B33_COMPONENT_BRIDGE_HPP

#    include <Borrowed.hpp>
#    include "IComponent.hpp"

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

    ~BorrowedComponent() noexcept
    {
        using ::std::exception;

        try
        {
            m_Component.Unlock();
        }
        catch ( const exception &e )
        {
            __B33_BEBUG_BREAK_POINT( e.what() )
        }
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

    using String           = ::std::string;
    using StringView       = ::std::string_view;
    using ComponentMapType = ::std::unordered_map<::std::string_view, ::std::unique_ptr<ComponentAbstractBase>>;

  public:
    ComponentBridge()
      : m_ComponentMap()
    {
    }

    ~ComponentBridge() noexcept = default;

    ComponentBridge( const ComponentBridge & ) = delete;
    ComponentBridge( ComponentBridge && )      = delete;

    ComponentBridge &operator=( const ComponentBridge & ) = delete;
    ComponentBridge &operator=( ComponentBridge && )      = delete;

  public:
    __B33_API ComponentAbstractBase &QueryComponent( String strComponentName )
    {
        B33_ASSERT( m_ComponentMap.find( strComponentName ) != m_ComponentMap.end() );
        return *m_ComponentMap[ strComponentName ].get();
    }

    ComponentAbstractBase *QueryComponentByClassName( StringView ComponentDerived )
    {
        B33_ASSERT( m_ComponentMap.find( ComponentDerived ) != m_ComponentMap.end() );
        ComponentAbstractBase *result = m_ComponentMap[ ComponentDerived ].get();
        return result;
    }

    template <class COMPONENT_DERIVED>
    Core::Borrowed<COMPONENT_DERIVED> QueryComponent()
    {
        B33_ASSERT( m_ComponentMap.find( COMPONENT_DERIVED::GetComponentName() ) != m_ComponentMap.end() );
        ComponentAbstractBase *result        = m_ComponentMap[ COMPONENT_DERIVED::GetComponentName() ].get();
        COMPONENT_DERIVED     *resultDerived = dynamic_cast<COMPONENT_DERIVED *>( result );
        return Core::Borrowed<COMPONENT_DERIVED>( &resultDerived->m_mUsed, resultDerived );
    }

  private:
    ComponentMapType m_ComponentMap = {};
};

} // namespace B33::System
#endif // !B33_COMPONENT_BRIDGE_HPP
