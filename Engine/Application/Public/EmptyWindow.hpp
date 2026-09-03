#if !defined( B33_EMPTYWINDOW_H )
#    define B33_EMPTYWINDOW_H

#    include "Window/IBaseWindow.hpp"

namespace B33::App
{

/**
 * Basic empty window implementation.
 * */
class EmptyWindow : public ::B33::App::IBaseWindow<EmptyWindow>
{
    using WString = ::std::wstring;

    friend class IBaseWindow<EmptyWindow>;

    template <typename T>
    constexpr decltype( auto ) Forward( T &arg ) noexcept
    {
        return ::std::forward<T>( arg );
    }

    template <typename T>
    constexpr decltype( auto ) Forward( T &&arg ) noexcept
    {
        return ::std::forward<T>( arg );
    }

  public:
    explicit EmptyWindow( WString wstrName = L"EmptyWindow" )
      : IBaseWindow<EmptyWindow>( CreateWindowDesc( std::move( wstrName ), 1200, 700 ) )
    {
    }

    template <class U>
    explicit EmptyWindow( U &&windowDesc )
      : IBaseWindow<EmptyWindow>( Forward<U>( windowDesc ) )
    {
    }

  private:
    virtual void HandleMessageImpl( const float, EB33WindowEventsFlags )
    {
        // B33_LOG(Debug::Info, L"Proccessing a message! ... %u", msg);
    }
};

} // namespace B33::App
#endif // !B33_EMPTYWINDOW_H
