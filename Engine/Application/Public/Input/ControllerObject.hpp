#if !defined( B33_CONTROLLER_OBJECT_HPP )
#    define B33_CONTROLLER_OBJECT_HPP

#    include "Input/UserInput.hpp"

namespace B33::App
{

/**
 * @class ControllerObject
 * @brief Unbinds itself from UserInput on destruction. Makes sure that destroyed objects aren't ever called inside UserInput.
 *
 */
class ControllerObject
{
    template <class T>
    using WeakPtr = ::std::weak_ptr<T>;

    friend UserInput;

  public:
    __B33_API ControllerObject() noexcept;

    __B33_API ~ControllerObject() noexcept;

  public:
    ControllerObject( const ControllerObject & )            = delete;
    ControllerObject &operator=( const ControllerObject & ) = delete;

    __B33_API                   ControllerObject( ControllerObject &&other ) noexcept;
    __B33_API ControllerObject &operator=( ControllerObject &&other ) noexcept;

  public:
    __B33_API void SignObject( WeakPtr<UserInput> pUserInput ) noexcept;

    __B33_API void UnsignObject() noexcept;

  private:
    WeakPtr<UserInput> m_pUserInput = {};
};

} // namespace B33::App

#endif // !B33_CONTROLLER_OBJECT_HPP
