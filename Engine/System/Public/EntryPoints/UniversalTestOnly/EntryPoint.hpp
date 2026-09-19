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

    usize uPassedTests;

    Logger::Get().Log( Info, L"---------------------------------------------" );
    Logger::Get().Log( Info, L"Running tests..." );

    uPassedTests = TestMaster::Get().Run();

    Logger::Get().Log( Info, L"Tests finished..." );
    Logger::Get().Flush();

    return ( uPassedTests != TestMaster::Get().GetTestAmount() );
}

#endif // !B33_ENTRY_POINT_HPP
