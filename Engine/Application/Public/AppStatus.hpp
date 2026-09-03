#if !defined( B33_APPSTATUS_H )
#    define B33_APPSTATUS_H

#    include <B33Core.h>

#    include "Window/WindowDesc.hpp"

namespace B33::App
{

enum EAppStatus
{
    Dead,
    Running,
    Service
};

class __B33_API AppStatus
{
    template <typename T, typename U>
    friend class IBaseWindow;

    template <typename T>
    using List = ::std::list<T>;

    template <typename T>
    using SharedPtr = ::std::shared_ptr<T>;

    AppStatus();

    AppStatus( const AppStatus & ) noexcept            = delete;
    AppStatus &operator=( const AppStatus & ) noexcept = delete;

    AppStatus( AppStatus && ) noexcept            = delete;
    AppStatus &operator=( AppStatus && ) noexcept = delete;

  public:
    static AppStatus &Get() noexcept;

    ~AppStatus() noexcept;

  public:
    static EAppStatus GetAppCurrentStatus();

    const List<SharedPtr<WindowDesc>> &GetWindowHandles() const
    {
        return m_WindowHandles;
    }

    void SendExitSignal();

    void LockInToService()
    {
        m_AppCurrentStatus = EAppStatus::Service;
    }

  private:
    uint32_t SendOpenWindowSignal( SharedPtr<WindowDesc> pWd );

    uint32_t SendCloseWindowSignal( SharedPtr<WindowDesc> pWd );

    void UpdateStatus();

  private:
    uint32_t m_uNumberOfWindows;

    List<SharedPtr<WindowDesc>> m_WindowHandles;

    inline static EAppStatus m_AppCurrentStatus = EAppStatus::Dead;
};

} // namespace B33::App
#endif // !B33_APPSTATUS_H
