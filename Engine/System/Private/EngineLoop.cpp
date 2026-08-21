#include "B33Core.h"
#include "B33System.hpp"
#include "AppStatus.hpp"
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
    m_Components[::B33::System::EComponentType::Default ] = vector<ComponentAbstractBase *> {};
    m_Components[::B33::System::EComponentType::Async ]   = vector<ComponentAbstractBase *> {};

    for ( auto &requiredComponent : m_ComponentOrderRegistry )
    {
        B33_INFO( L"Adding compontent %s", requiredComponent.data() );
        AddComponentInternal( requiredComponent );
    }

    m_JobSystem.BlockAndWait();
}

void EngineLoop::UpdateComponents( float fDelta )
{
#if defined( _B33_DEBUG )
    try
    {
#endif
        constexpr auto asyncCall = +[]( ComponentAbstractBase *pComponent, float fDelta, ComponentBridge *pBridge )
        {
            pComponent->Lock();
            dynamic_cast<ComponentAsync *>( pComponent )->Update( fDelta, *pBridge );
            pComponent->DecreaseCount();
            pComponent->Unlock();
        };

        B33_TRACE( L"Starting update loop" );
        for ( auto [ componentType, componentVector ] : m_Components )
        {
            B33_TRACE( L"Updateing component type: %d", componentType );
            switch ( componentType )
            {
                case Default:
                    for ( auto *component : componentVector )
                    {
                        component->Lock();
                        dynamic_cast<Component *>( component )->Update( fDelta, m_ComponentBridge );
                        component->Unlock();
                    }
                    continue;
                case Async:
                    for ( ComponentAbstractBase *component : componentVector )
                    {
                        if ( component->GetCount() < 2 )
                        {
                            B33_TRACE( L"Queue job for component %p", component );
                            component->IncreaseCount();
                            m_JobSystem.PushJob( asyncCall, component, fDelta, &m_ComponentBridge );
                        }
                        else
                        {
                            B33_TRACE( L"Queued more then 2 jobs for the component, skipping" );
                        }
                    }
                    continue;
                default:
                    B33_ASSERT_MSG( false, "Unknown component type" );
            }
        }
        m_JobSystem.BlockAndWait();
#if defined( _B33_DEBUG )
    }
    catch ( ... )
    {
        throw B33_EXCEPT("Error during components update");
    }
#endif
}

void EngineLoop::DestroyComponents()
{
    m_JobSystem.BlockAndWait();

    for ( ComponentsMap::reverse_iterator it = m_Components.rbegin(); it != m_Components.rend(); ++it )
    {
        auto [ componentType, componentVector ] = *it;

        switch ( componentType )
        {
            case Default:
            {
                for ( auto back = componentVector.rbegin(); back != componentVector.rend(); ++back )
                {
                    dynamic_cast<Component *>( *back )->Destroy( m_ComponentBridge );
                }
                continue;
            }
            case Async:
            {
                for ( auto back = componentVector.rbegin(); back != componentVector.rend(); ++back )
                {
                    dynamic_cast<ComponentAsync *>( *back )->Destroy( m_ComponentBridge );
                }
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
    constexpr auto asyncCall = +[]( ComponentAbstractBase *pComponent, ComponentBridge *pBridge )
    {
        pComponent->Lock();
        dynamic_cast<ComponentAsync *>( pComponent )->Initialize( *pBridge );
        pComponent->Unlock();
    };

    B33_ASSERT_MSG( m_ComponentRegistry.find( componentName ) != m_ComponentRegistry.end(),
                    "That component isn't registered. B33COMPONENT macro might be missing in the class body. " );

    m_ComponentBridge.m_ComponentMap[ componentName ] = m_ComponentRegistry[ componentName ]();

    auto component = ( m_ComponentBridge.m_ComponentMap[ componentName ] ).get();
    switch ( component->GetComponentType() )
    {
        case Default:
        {
            if ( m_bInitialized )
            {
                component->Lock();
                component->Initialize( m_ComponentBridge );
                component->Unlock();
            }

            m_Components[::B33::System::EComponentType::Default ].push_back( component );
            break;
        }
        case Async:
        {
            if ( m_bInitialized )
                m_JobSystem.PushJob( asyncCall, component, &m_ComponentBridge );

            m_Components[::B33::System::EComponentType::Async ].push_back( component );
            break;
        }
        case AsyncUpdateOnly:
        {
            if ( m_bInitialized )
            {
                component->Lock();
                component->Initialize( m_ComponentBridge );
                component->Unlock();
            }

            m_Components[::B33::System::EComponentType::Async ].push_back( component );
            break;
        }
        default:
        {
            B33_ASSERT_MSG( false, "Unknown component type" );
        }
    }
}

} // namespace B33::System
