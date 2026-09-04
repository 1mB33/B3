#include "Tests/TestMaster.hpp"
#include "TerminalColors.hpp"

namespace B33::Core::Tests
{

// --------------------------------------------------------------------------------------------------------------------
TestMaster &TestMaster::Get()
{
    static TestMaster instance;
    return instance;
}

// ---------------------------------------------------------------------------------------------------------------------
TestMaster::TestMaster()
  : m_uTestAmount( 0 )
  , m_pTestsBuf()
{
}

// ---------------------------------------------------------------------------------------------------------------------
void TestMaster::AddTest( void ( *pTest )(), const char *pszTestName, usize uTestNameLen )
{
    m_pTestsBuf.push_back( Test { pTest, pszTestName, uTestNameLen, false } );
    ++m_uTestAmount;
}

// ---------------------------------------------------------------------------------------------------------------------
void TestMaster::Run()
{
    Debug::Logger::Get().Log( Debug::Info, L"Starting tests..." );

    u32 uTestsPassed = 0;
    for ( usize i = 0; i < m_uTestAmount; ++i )
    {
        try
        {
            Debug::Logger::Get().Log( Debug::Info, L"Starting test: %s", m_pTestsBuf[ i ].pszTestName );
            m_pTestsBuf[ i ].pTest();

            m_pTestsBuf[ i ].bPassed = true;
            ++uTestsPassed;

            Debug::Logger::Get().Log( Debug::Info, L"Passed test: %s", m_pTestsBuf[ i ].pszTestName );
        }
        catch ( ... )
        {
            m_pTestsBuf[ i ].bPassed = false;

            Debug::Logger::Get().Log( Debug::Info,
#if defined( _WIN32 )
                                      L"Failed test: %S",
#elif defined( __linux__ ) || defined( __APPLE__ )
                                      L"Failed test: %s",
#endif // !_WIN32
                                      ColorizeTerminal::Colorize( m_pTestsBuf[ i ].pszTestName, BrightRed ).c_str() );
        }
    }

    // for (USIZE i = 0; i < m_uTestAmount; ++i) {
    //     if (m_pTestsBuf[i].bPassed) {
    //         continue;
    //     }
    // }

    Debug::Logger::Get().Log( Debug::Info, L"Passed tests %d out of %d", uTestsPassed, m_uTestAmount );
}

} // namespace B33::Core::Tests
