#if !defined(B33_CONTROLLER_OBJECT_HPP)
#define B33_CONTROLLER_OBJECT_HPP

#include "B33Core.h"

#include "Bind.h"
#include "Input/UserInput.hpp"

namespace B33::App
{

class ControllerObject
{
    friend UserInput;

  public:
    ControllerObject() = default;

    __B33_API ~ControllerObject() noexcept;

  public:
    ControllerObject( const ControllerObject & ) noexcept            = delete;
    ControllerObject &operator=( const ControllerObject & ) noexcept = delete;

    __B33_API ControllerObject( ControllerObject &&other ) noexcept;

  public:
    __B33_API void SignObject( ::std::weak_ptr<UserInput> pUserInput );

  private:
    ::std::weak_ptr<UserInput> m_pUserInput;
};

} // namespace B33::App

#endif // !B33_CONTROLLER_OBJECT_HPP
