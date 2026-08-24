#if defined( __linux__ )
#    ifndef B33_ENTRY_POINT_HPP
#        define B33_ENTRY_POINT_HPP

#        include "../../B33System.hpp"
#        include "Synchronization/DeltaTime.hpp"
#        include "../../EngineLoop.hpp"
#        include "Tests/TestMaster.hpp"

int main( int, char *[] )
{
    ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info, L"---------------------------------------------" );
    ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info, L"Starting B33..." );
#        if defined( _B33_ONLY_TESTS )
    ::B33::Core::Tests::TestMaster::Get().Run();

    ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info, L"Closing B33..." );
    ::B33::Core::Debug::Logger::Get().Flush();
    return 0;
#        endif // defined (_B33_ONLY_TESTS)
#        if defined( _B33_TESTS )
    ::B33::Core::Tests::TestMaster::Get().Run();
#        endif // defined (_B33_TESTS)

    ::B33::System::EngineLoop engineLoop = {};
    ::B33::Core::DeltaTime    dt         = {};

#        if defined( _B33_DEBUG )
    ::B33::Core::Debug::Logger::Get().Flush();
    try
    {
#        endif
        engineLoop.InitializeComponents();

        ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info, L"B33 started..." );
        dt.SetReferenceFrame();
        while ( ::B33::App::AppStatus::GetAppCurrentStatus() )
        {
            engineLoop.UpdateComponents( dt.FetchMs() );
        }

        engineLoop.DestroyComponents();

#        if defined( _B33_DEBUG )
    }
    catch ( ... )
    {
        ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Error, L"Internal error, closing!" );
    }
#        endif

    ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info, L"Closing B33..." );
    ::B33::Core::Debug::Logger::Get().Flush();
}

#    endif // !B33_ENTRY_POINT_HPP
#endif     // !__linux__
