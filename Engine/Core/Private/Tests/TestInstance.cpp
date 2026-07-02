#include "Tests/TestInstance.hpp"
#include "Tests/TestMaster.hpp"

namespace B33::Core::Tests
{

// ---------------------------------------------------------------------------------------------------------------------
TestInstance::TestInstance( void( *pTest )(), const char szName[] )
{
    TestMaster::Get().AddTest( pTest, szName, strlen( szName ) );
}

} // namespace B33::Core::Tests
