#include "B33App.h"

#include "AppResources.hpp"
#include "AppStatus.hpp"

namespace B33::App
{

using namespace ::std;
using namespace ::B33::Core;
using namespace ::B33::Core::Debug;

// --------------------------------------------------------------------------------------------------------------------
AppStatus::AppStatus()
  : m_uNumberOfWindows( 0 )
  , m_WindowHandles()
{
}

// --------------------------------------------------------------------------------------------------------------------
AppStatus::~AppStatus() noexcept {}

// --------------------------------------------------------------------------------------------------------------------
AppStatus &AppStatus::Get() noexcept
{
    static AppStatus instance;
    return instance;
}

// --------------------------------------------------------------------------------------------------------------------
EAppStatus AppStatus::GetAppCurrentStatus()
{
    return m_AppCurrentStatus;
}

// --------------------------------------------------------------------------------------------------------------------
void AppStatus::SendExitSignal()
{
    B33_LOG( Warning, L"B33::AppStatus Exiting [Number of active windows: %d]", m_uNumberOfWindows );

    if ( m_WindowHandles.empty() )
    {
        B33_LOG( Error, L"B33::AppStatus m_WindowHandles is empty!!!" );
    }

    while ( !m_WindowHandles.empty() )
    {
        SendCloseWindowSignal( m_WindowHandles.front() );
    }

    UpdateStatus();
}

// --------------------------------------------------------------------------------------------------------------------
u32 AppStatus::SendOpenWindowSignal( AppStatus::SharedPtr<WindowDesc> pWd )
{
    B33_LOG( Info, L"Got new window signal" );

    ++m_uNumberOfWindows;

    UpdateStatus();
    m_WindowHandles.push_back( pWd );

    return m_uNumberOfWindows;
}

// --------------------------------------------------------------------------------------------------------------------
u32 AppStatus::SendCloseWindowSignal( AppStatus::SharedPtr<WindowDesc> pWd )
{
    B33_LOG( Info, L"Got close window signal" );

    m_uNumberOfWindows = m_uNumberOfWindows > 0 ? ( m_uNumberOfWindows - 1 ) : m_uNumberOfWindows;
    UpdateStatus();
    for ( auto it = m_WindowHandles.begin(); it != m_WindowHandles.end(); ++it )
    {
        if ( *it == pWd )
        {
            it = m_WindowHandles.erase( it );
            break;
        }
    }

    return m_uNumberOfWindows;
}

// --------------------------------------------------------------------------------------------------------------------
void AppStatus::UpdateStatus()
{
    B33_LOG( Info, L"B33::AppStatus updated [Number of active windows: %d]", m_uNumberOfWindows );

    if ( m_uNumberOfWindows == 0 )
    {
        m_AppCurrentStatus = EAppStatus::Dead;
        return;
    }

    m_AppCurrentStatus = EAppStatus::Running;
}

} // namespace B33::App
