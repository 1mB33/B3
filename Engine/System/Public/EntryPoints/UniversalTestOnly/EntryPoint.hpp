#ifndef B33_ENTRY_POINT_HPP
#define B33_ENTRY_POINT_HPP

#include <Debug/Logger.hpp>
#include "Tests/TestMaster.hpp"

int main( int, char *[] )
{
    using ::B33::Core::Debug::Error;
    using ::B33::Core::Debug::Info;
    using ::B33::Core::Debug::Logger;
    using ::B33::Core::Tests::TestMaster;

    Logger::Get().Log( Info, L"---------------------------------------------" );
    Logger::Get().Log( Info, L"Starting B33..." );
#if defined( _B33_ONLY_TESTS )
    TestMaster::Get().Run();

    Logger::Get().Log( Info, L"Closing B33..." );
    Logger::Get().Flush();
    return 0;
#endif // defined (_B33_ONLY_TESTS)
}

#endif // !B33_ENTRY_POINT_HPP
