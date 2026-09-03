#if !defined( B33_WINDOW_LISTENER_H )
#    define B33_WINDOW_LISTENER_H

#    include <B33Core.h>

#    include "Window/WindowDesc.hpp"

namespace B33::App
{

class WindowListener
{
    template <typename T>
    using SharedPtr = ::std::shared_ptr<T>;

  public:
    explicit WindowListener( SharedPtr<WindowDesc> pWd = nullptr )
      : m_pWindowDesc( pWd )
    {
    }

    ~WindowListener() noexcept = default;

  public:
    WindowListener( const WindowListener &other ) noexcept
      : m_pWindowDesc( other.m_pWindowDesc )
    {
    }

    WindowListener &operator=( const WindowListener &other ) noexcept
    {
        this->m_pWindowDesc = other.m_pWindowDesc;
        return *this;
    }

    WindowListener( WindowListener &&other ) noexcept
      : m_pWindowDesc( std::move( other.m_pWindowDesc ) )
    {
    }

    WindowListener &operator=( WindowListener &&other ) noexcept
    {
        this->m_pWindowDesc = std::move( other.m_pWindowDesc );
        return *this;
    }

  public:
    void ListenToWindow( SharedPtr<WindowDesc> pWd )
    {
        B33_ASSERT( pWd != nullptr );
        m_pWindowDesc = pWd;
    }

    void ListenToWindow( WindowListener &other )
    {
        m_pWindowDesc = other.m_pWindowDesc;
    }

  protected:
    WindowDesc *GetWindowDesc() const
    {
        B33_ASSERT( m_pWindowDesc != nullptr );
        return m_pWindowDesc.get();
    }

  private:
    SharedPtr<WindowDesc> m_pWindowDesc = nullptr;
};

} // namespace B33::App
#endif //! B33_WINDOW_LISTENER_H
