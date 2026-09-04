#if !defined( B3_TESTMASTER_HPP )
#    define B3_TESTMASTER_HPP

namespace B33::Core::Tests

{
class TestMaster
{
    template <typename T>
    using Vector = ::std::vector<T>;

    __B33_API TestMaster();

    TestMaster( const TestMaster & ) = delete;
    TestMaster( TestMaster && )      = delete;

    TestMaster &operator=( const TestMaster & ) = delete;
    TestMaster &operator=( TestMaster && )      = delete;

  public:
    __B33_API static TestMaster &Get();

    ~TestMaster() noexcept = default;

  public:
    __B33_API void AddTest( void ( *pTest )(), const char *pszTestName, usize uTestNameLen );

    __B33_API void Run();

  private:
    struct Test
    {
        void ( *pTest )();
        const char *pszTestName;
        usize       uTestNameLen;
        bool        bPassed;
    };

  private:
    usize        m_uTestAmount = -1;
    Vector<Test> m_pTestsBuf   = {};
};

} // namespace B33::Core::Tests
#endif // !B3_TESTMASTER_HPP
