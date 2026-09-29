#if !defined( B33_ICALL_HPP )
#    define B33_ICALL_HPP

namespace B33::System
{
class ICall
{
  public:
    ICall() noexcept          = default;
    virtual ~ICall() noexcept = default;

  public:
    ICall( ICall && ) noexcept                 = default;
    ICall &operator=( ICall && ) noexcept      = default;
    ICall( const ICall & ) noexcept            = default;
    ICall &operator=( const ICall & ) noexcept = default;

  public:
    virtual inline void OnCall() = 0;
};

} // namespace B33::System

#endif
