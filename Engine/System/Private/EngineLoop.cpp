#include "B33Core.h"
#include "B33System.hpp"
#include "ComponentBridge.hpp"
#include "Debug/Assert.hpp"
#include "EngineLoop.hpp"
#include "IComponent.hpp"

namespace B33::System
{

using namespace std;
using namespace B33;

void EngineLoop::InitializeComponents()
{
    m_bInitialized = true;
    m_Components.clear();
    m_Components[::B33::System::EComponentType::Async ]         = vector<IComponentAbstractBase *> {};
    m_Components[::B33::System::EComponentType::AsyncNoBridge ] = vector<IComponentAbstractBase *> {};
    m_Components[::B33::System::EComponentType::NoBridge ]      = vector<IComponentAbstractBase *> {};
    m_Components[::B33::System::EComponentType::Default ]       = vector<IComponentAbstractBase *> {};

    for ( auto &requiredComponent : m_ComponentOrderRegistry )
        AddComponentInternal( requiredComponent );
}

void EngineLoop::UpdateComponents( float fDelta )
{
    B33_TRACE( L"Starting update loop" );
    for ( auto [ componentType, componentVector ] : m_Components )
    {
        B33_TRACE( L"Updateing component type: %d", componentType );
        switch ( componentType )
        {
            case Default:
            {
                for ( auto component : componentVector )
                    dynamic_cast<IComponentDefault *>( component )->Update( fDelta, m_ComponentBridge );
                continue;
            }
            case Async:
            {
                for ( IComponentAbstractBase *component : componentVector )
                    m_JobSystem.PushJob(
                        [ = ]()
                        {
                            dynamic_cast<IComponentAsync *>( component )->Update( fDelta );
                        } );
                continue;
            }
            case AsyncNoBridge:
            {
                for ( IComponentAbstractBase *component : componentVector )
                    m_JobSystem.PushJob(
                        [ = ]()
                        {
                            dynamic_cast<IComponentAsyncNoBridge *>( component )->Update( fDelta );
                        } );
                continue;
            }
            case NoBridge:
            {
                m_JobSystem.BlockAndWait();

                for ( IComponentAbstractBase *component : componentVector )
                    dynamic_cast<IComponentNoBridge *>( component )->Update( fDelta );
                continue;
            }
            default:
            {
                B33_ASSERT_MSG( false, "Unknown component type" );
            }
        }
    }
}

void EngineLoop::DestroyComponents()
{
    for ( ComponentsMap::reverse_iterator it = m_Components.rbegin(); it != m_Components.rend(); ++it )
    {
        auto [ componentType, componentVector ] = *it;

        switch ( componentType )
        {
            case Default:
            {
                for ( auto back = componentVector.rbegin(); back != componentVector.rend(); ++back )
                    dynamic_cast<IComponentDefault *>( *back )->Destroy( m_ComponentBridge );
                continue;
            }
            case Async:
            {
                for ( auto back = componentVector.rbegin(); back != componentVector.rend(); ++back )
                    dynamic_cast<IComponentAsync *>( *back )->Destroy( m_ComponentBridge );
                continue;
            }
            case AsyncNoBridge:
            {
                for ( auto back = componentVector.rbegin(); back != componentVector.rend(); ++back )
                    dynamic_cast<IComponentAsyncNoBridge *>( *back )->Destroy();
                continue;
            }
            case NoBridge:
            {
                for ( auto back = componentVector.end() - 1; back >= componentVector.begin(); --back )
                    dynamic_cast<IComponentNoBridge *>( *back )->Destroy();
                continue;
            }
            default:
            {
                B33_ASSERT_MSG( false, "Unknown component type" );
            }
        }
    }
}

void EngineLoop::AddComponentInternal( ::std::string_view componentName )
{
    B33_ASSERT_MSG( m_ComponentRegistry.find( componentName ) != m_ComponentRegistry.end(),
                    "That component isn't registered. B33COMPONENT macro might be missing in the class body. " );

    m_ComponentBridge.m_ComponentMap[ componentName ] = m_ComponentRegistry[ componentName ]();

    auto component = ( m_ComponentBridge.m_ComponentMap[ componentName ] ).get();
    switch ( component->GetComponentType() )
    {
        case Default:
        {
            if ( m_bInitialized )
                component->Initialize( m_ComponentBridge );

            m_Components[::B33::System::EComponentType::Default ].push_back( component );
            break;
        }
        case Async:
        {
            if ( m_bInitialized )
                component->Initialize( m_ComponentBridge );

            m_Components[::B33::System::EComponentType::Async ].push_back( component );
            break;
        }
        case AsyncNoBridge:
        {
            if ( m_bInitialized )
                dynamic_cast<IComponentAsyncNoBridge *>( component )->Initialize();

            m_Components[::B33::System::EComponentType::AsyncNoBridge ].push_back( component );
            break;
        }
        case NoBridge:
        {
            if ( m_bInitialized )
                dynamic_cast<IComponentNoBridge *>( component )->Initialize();

            m_Components[::B33::System::EComponentType::NoBridge ].push_back( component );
            break;
        }

        default:
        {
            B33_ASSERT_MSG( false, "Unknown component type" );
        }
    }
}

} // namespace B33::System
