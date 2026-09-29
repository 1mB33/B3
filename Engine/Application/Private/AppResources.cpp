#include "B33App.h"

#include "AppResources.hpp"

namespace B33::App
{

using namespace ::std;
using namespace ::B33::Core;
using namespace ::B33::Core::Debug;

__B33_ATTRIBUTE_MIGHT_BE_UNUSED static struct Runnable
{
    Runnable()
    {
        B33_INFO( L"Spawing AppResources on main thread" );
        AppResources::Get();
    }
} MakeSureThatAppResourcesAreOnMainThread = {};

// ---------------------------------------------------------------------------------------------------------------------
AppResources::AppResources()
  : m_wstrExePathW( InternalGetExecutablePathW() )
  , m_strExePathA( InternalGetExecutablePathA( m_wstrExePathW ) )
  , m_MainThreadId( this_thread::get_id() )
{
}

// Public // ----------------------------------------------------------------------------------------------------------
const AppResources::WString &AppResources::GetExecutablePathW() const
{
    return m_wstrExePathW;
}

const AppResources::String &AppResources::GetExecutablePathA() const
{
    return m_strExePathA;
}

const thread::id &AppResources::GetMainThreadID() const
{
    return m_MainThreadId;
}

// ---------------------------------------------------------------------------------------------------------------------
AppResources &AppResources::Get()
{
    static AppResources instance;
    return instance;
}

// Private // ---------------------------------------------------------------------------------------------------------
#if defined( __linux__ )
AppResources::WString AppResources::InternalGetExecutablePathW()
{
    char    sPath[ B33_LONG_STRING ];
    ssize_t uLen = readlink( "/proc/self/exe", sPath, sizeof( sPath ) - 1 );

    if ( uLen == -1 )
    {
        B33_LOG( Error, L"Couldn't get path to executable!!! Falling back to `./`" );
        return wstring( L"./" );
    }

    sPath[ uLen ] = '\0';

    wstring wstrPath( uLen + 1, {} );
    mbstowcs( wstrPath.data(), sPath, uLen );

    return wstrPath.substr( 0, wstrPath.find_last_of( '/' ) );
}

#elif defined( __APPLE__ )
// ---------------------------------------------------------------------------------------------------------------------
AppResources::WString AppResources::InternalGetExecutablePathW()
{
    char     sPath[ B33_LONG_STRING ];
    uint32_t uLen = B33_LONG_STRING;

    if ( _NSGetExecutablePath( sPath, &uLen ) )
    {
        B33_LOG( Error, L"Couldn't get path to executable!!! Falling back to `./`" );
        return wstring( L"./" );
    }

    puts( sPath );

    wstring wstrPath( uLen + 1, {} );
    mbstowcs( wstrPath.data(), sPath, uLen );

    return wstrPath.substr( 0, wstrPath.find_last_of( '/' ) );
}

#elif _WIN32
// --------------------------------------------------------------------------------------------------------------------
AppResources::WString AppResources::InternalGetExecutablePathW()
{
    return wstring( L"./" );
}
#endif // !__linux__

// --------------------------------------------------------------------------------------------------------------------
AppResources::String AppResources::InternalGetExecutablePathA( const AppResources::WString wstrBase )
{
    size_t len = wcstombs( nullptr, wstrBase.c_str(), 0 );

    if ( len != static_cast<size_t>( -1 ) )
    {
        string str( len, '\0' );
        wcstombs( &str[ 0 ], wstrBase.c_str(), len );
        return str;
    }

    return "./";
}

} // namespace B33::App
