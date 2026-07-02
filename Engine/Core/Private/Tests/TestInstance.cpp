#include "B33Core.h"
#include "Tests/TestInstance.hpp"
#include "Tests/TestMaster.hpp"

namespace B33::Core::Tests
{

// ---------------------------------------------------------------------------------------------------------------------
TestInstance::TestInstance( void ( *pTest )(), const char szName[] )
{
    B33_TRACE( L"Collecting test" );
    TestMaster::Get().AddTest( pTest, szName, strlen( szName ) );
}

} // namespace B33::Core::Tests
