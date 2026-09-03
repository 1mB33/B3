#if !defined( B33_EMPTYCANVAS_H )
#    define B33_EMPTYCANVAS_H

#    include <B33Core.h>
#    include "Input/UserInput.hpp"
#    include "Window/IBaseWindow.hpp"
#    include "Window/WindowPolicy/GameSystemPolicy.hpp"

namespace B33::App
{

/**
 * Basic empty window implementation for rendering and game. Has it's own built in UserInput instance.
 *
 * @param bManualInputUpdate - if true, input update will not be called automatically on every window update,
 * this way you can manualy update the input on another thread or at a different time.
 * @param GamePolicy - policy that defines window behaviour on different os level events. Defaults to
 * DefaultGameSystemWindowPolicy
 * */
template <bool bManualInputUpdate = false, class GamePolicy = DefaultGameSystemWindowPolicy>
class EmptyCanvas : public ::B33::App::IBaseWindow<EmptyCanvas<bManualInputUpdate>, GamePolicy>
{
    template <typename T>
    using SharedPtr = ::std::shared_ptr<T>;

    template <typename T>
    using WeakPtr = ::std::weak_ptr<T>;

    using WString = ::std::wstring;

    friend class IBaseWindow<EmptyCanvas<bManualInputUpdate>, GamePolicy>;

    template <typename T>
    constexpr decltype( auto ) MakeShared() noexcept
    {
        return ::std::make_shared<T>();
    }

    template <typename T, typename U>
    constexpr decltype( auto ) MakeShared( U &&arg ) noexcept
    {
        return ::std::make_shared<T>( Forward<U>( arg ) );
    }

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
    EmptyCanvas( WString wstrWindowName, i32 defaultWidth, i32 defaultHeigth )
      : IBaseWindow<EmptyCanvas, GamePolicy>(
            CreateWindowDesc( ::std::move( wstrWindowName ), defaultWidth, defaultHeigth ) )
      , m_pInput( MakeShared<UserInput>( this->GetWindowDesc() ) )
    {
    }

    template <class U>
    explicit EmptyCanvas( U &&desc )
      : IBaseWindow<EmptyCanvas, GamePolicy>( Forward<U>( desc ) )
    {
    }

  public:
    WeakPtr<UserInput> GetInput() const
    {
        return m_pInput;
    }

  private:
    /**
     * @brief It's called on every update
     */
    void HandleMessageImpl( const float fDelta, __B33_ATTRIBUTE_MIGHT_BE_UNUSED EB33WindowEventsFlags events )
    {
        if constexpr ( !bManualInputUpdate )
        {
            m_pInput->Update( fDelta );
        }
    }

  private:
    SharedPtr<UserInput> m_pInput;
};

} // namespace B33::App
#endif // !B33_EMPTYCANVAS_H
