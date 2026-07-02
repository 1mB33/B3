#include "Debug/Logger.hpp"
#include "Tests/TestMaster.hpp"

namespace B33::Core::Tests
{

// ---------------------------------------------------------------------------------------------------------------------
TestMaster::TestMaster()
  : m_uTestAmount( 0 )
  , m_pTestsBuf()
{
}

// ---------------------------------------------------------------------------------------------------------------------
void TestMaster::AddTest( void( *pTest )(), const char *pszTestName, size_t uTestNameLen )
{
    m_pTestsBuf[ m_uTestAmount++ ] = Test { pTest, pszTestName, uTestNameLen, false };
}

// ---------------------------------------------------------------------------------------------------------------------
void TestMaster::Run()
{
    Debug::Logger::Get().Log( Debug::Info, L"Starting tests..." );

    uint32_t uTestsPassed = 0;
    for ( size_t i = 0; i < m_uTestAmount; ++i )
    {
        try
        {
            m_pTestsBuf[ i ].pTest();

            m_pTestsBuf[ i ].bPassed = true;
            ++uTestsPassed;

            Debug::Logger::Get().Log( Debug::Info, L"Passed test: %s", m_pTestsBuf[ i ].pszTestName );
        }
        catch ( ... )
        {
            m_pTestsBuf[ i ].bPassed = false;

            Debug::Logger::Get().Log( Debug::Info, L"Failed test: %s", m_pTestsBuf[ i ].pszTestName );
        }
    }

    // for (USIZE i = 0; i < m_uTestAmount; ++i) {
    //     if (m_pTestsBuf[i].bPassed) {
    //         continue;
    //     }
    // }

    Debug::Logger::Get().Log( Debug::Info, L"Passed test %d out of %d", uTestsPassed, m_uTestAmount );
}

} // namespace B33::Core::Tests
