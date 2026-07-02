#ifndef B3_TESTMASTER_HPP
#define B3_TESTMASTER_HPP

namespace B33::Core::Tests

{
class __B33_API TestMaster
{
    TestMaster();

    TestMaster( const TestMaster & ) = delete;
    TestMaster( TestMaster && )      = delete;

    TestMaster &operator=( const TestMaster & ) = delete;
    TestMaster &operator=( TestMaster && )      = delete;

  public:
    static TestMaster &Get()
    {
        static TestMaster instance = {};
        return instance;
    }

    ~TestMaster() = default;

  public:
    void AddTest( void ( *pTest )(), const char *pszTestName, size_t uTestNameLen );

    void Run();

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
