#if !defined(B33_APP_RESOURCES_H)
#define B33_APP_RESOURCES_H

#include "B33Core.h"

namespace B33::App
{

class AppResources
{
    __B33_API AppResources();

  public:
    ~AppResources() = default;

    AppResources( AppResources && )            = delete;
    AppResources &operator=( AppResources && ) = delete;

    AppResources( const AppResources & )            = delete;
    AppResources &operator=( const AppResources & ) = delete;

    // Getters // -----------------------------------------------------------------------------------------------------
  public:
    __B33_API const ::std::wstring &GetExecutablePathW() const;

    __B33_API const ::std::string &GetExecutablePathA() const;

    __B33_API const ::std::thread::id &GetMainThreadID() const;

    // Methods // -----------------------------------------------------------------------------------------------------
  public:
    __B33_API static AppResources &Get();

  private:
    static ::std::wstring InternalGetExecutablePathW();

    static ::std::string InternalGetExecutablePathA( const ::std::wstring wstrBase );

  private:
    ::std::wstring    m_wstrExePathW;
    ::std::string     m_strExePathA;
    ::std::thread::id m_MainThreadId;
};

} // namespace B33::App
#endif // !B33_APP_RESOURCES_H
