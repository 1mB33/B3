#if defined( __linux__ )
#    ifndef B33_ENTRY_POINT_HPP
#        define B33_ENTRY_POINT_HPP

#        include <AppStatus.hpp>
#        include "Synchronization/DeltaTime.hpp"
#        include "../../EngineLoop.hpp"
#        include "Tests/TestMaster.hpp"

int main( int, char *[] )
{
    using ::B33::App::AppStatus;
    using ::B33::Core::DeltaTime;
    using ::B33::Core::Debug::Error;
    using ::B33::Core::Debug::Info;
    using ::B33::Core::Debug::Logger;
    using ::B33::Core::Tests::TestMaster;
    using ::B33::System::EngineLoop;

    Logger::Get().Log( Info, L"---------------------------------------------" );
    Logger::Get().Log( Info, L"Starting B33..." );
#        if defined( _B33_ONLY_TESTS )
    TestMaster::Get().Run();

    Logger::Get().Log( Info, L"Closing B33..." );
    Logger::Get().Flush();
    return 0;
#        endif // defined (_B33_ONLY_TESTS)
#        if defined( _B33_TESTS )
    TestMaster::Get().Run();
#        endif // defined (_B33_TESTS)

    EngineLoop engineLoop = {};
    DeltaTime  dt         = {};

#        if defined( _B33_DEBUG )
    Logger::Get().Flush();
    try
    {
#        endif
        engineLoop.InitializeComponents();

        Logger::Get().Log( Info, L"B33 started..." );
        dt.SetReferenceFrame();
        while ( AppStatus::GetAppCurrentStatus() )
        {
            engineLoop.UpdateComponents( dt.FetchMs() );
        }

        engineLoop.DestroyComponents();

#        if defined( _B33_DEBUG )
    }
    catch ( ... )
    {
        Logger::Get().Log( Error, L"Internal error, closing!" );
    }
#        endif

    Logger::Get().Log( Info, L"Closing B33..." );
    Logger::Get().Flush();
}

#    endif // !B33_ENTRY_POINT_HPP
#endif     // !__linux__
