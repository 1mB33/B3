#if defined( _WIN32 )
#    ifndef B33_ENTRY_POINT_HPP
#        define B33_ENTRY_POINT_HPP

#        include "../../B33System.hpp"
#        include "Synchronization/DeltaTime.hpp"
#        include "../../EngineLoop.hpp"
#        include "Tests/TestMaster.hpp"

int WINAPI wWinMain( HINSTANCE, HINSTANCE, PWSTR, int )
{
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

    engineLoop.InitializeComponents();

    ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info, L"B33 started..." );
    dt.SetReferenceFrame();
    while ( ::B33::App::AppStatus::GetAppCurrentStatus() )
    {
        engineLoop.UpdateComponents( dt.FetchMs() );
    }

    engineLoop.DestroyComponents();

    ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info, L"Closing B33..." );
    ::B33::Core::Debug::Logger::Get().Flush();

    return 0;
}

#    endif // !B33_ENTRY_POINT_HPP
#endif     // !_WIN32
