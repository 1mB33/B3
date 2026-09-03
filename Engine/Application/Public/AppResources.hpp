#if !defined( B33_APP_RESOURCES_H )
#    define B33_APP_RESOURCES_H

#    include <B33Core.h>

namespace B33::App
{

class AppResources
{
    using String   = ::std::string;
    using WString  = ::std::wstring;
    using ThreadID = ::std::thread::id;

    __B33_API AppResources();

  public:
    ~AppResources() noexcept = default;

    AppResources( AppResources && )            = delete;
    AppResources &operator=( AppResources && ) = delete;

    AppResources( const AppResources & )            = delete;
    AppResources &operator=( const AppResources & ) = delete;

    // Getters // -----------------------------------------------------------------------------------------------------
  public:
    __B33_API const WString &GetExecutablePathW() const;

    __B33_API const String &GetExecutablePathA() const;

    __B33_API const ThreadID &GetMainThreadID() const;

    // Methods // -----------------------------------------------------------------------------------------------------
  public:
    __B33_API static AppResources &Get();

  private:
    static WString InternalGetExecutablePathW();

    static String InternalGetExecutablePathA( const WString wstrBase );

  private:
    WString  m_wstrExePathW;
    String   m_strExePathA;
    ThreadID m_MainThreadId;
};

} // namespace B33::App
#endif // !B33_APP_RESOURCES_H
