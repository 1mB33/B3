#if !defined(B3_TESTMASTER_HPP)
#define B3_TESTMASTER_HPP

namespace B33::Core::Tests

{
class TestMaster
{
    __B33_API TestMaster();

    TestMaster( const TestMaster & ) = delete;
    TestMaster( TestMaster && )      = delete;

    TestMaster &operator=( const TestMaster & ) = delete;
    TestMaster &operator=( TestMaster && )      = delete;

  public:
    __B33_API static TestMaster &Get();

    ~TestMaster() = default;

  public:
    __B33_API void AddTest( void ( *pTest )(), const char *pszTestName, size_t uTestNameLen );

    __B33_API void Run();

  private:
    struct Test
    {
        void ( *pTest )();
        const char *pszTestName;
        size_t      uTestNameLen;
        bool        bPassed;
    };

  private:
    size_t              m_uTestAmount = -1;
    ::std::vector<Test> m_pTestsBuf   = {};
};

} // namespace B33::Core::Tests
#endif // !B3_TESTMASTER_HPP
