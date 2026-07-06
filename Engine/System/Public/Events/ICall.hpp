#if !defined( B33_ICALL_HPP )
#    define B33_ICALL_HPP

namespace B33::System
{
class ICall
{
  public:
    ICall()          = default;
    virtual ~ICall() = default;

  public:
    ICall( ICall && )                 = default;
    ICall &operator=( ICall && )      = default;
    ICall( const ICall & )            = default;
    ICall &operator=( const ICall & ) = default;

  public:
    virtual inline void OnCall() = 0;
};

} // namespace B33::System

#endif
