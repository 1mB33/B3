#if !defined( B33_EVENT_TYPE_HPP )
#    define B33_EVENT_TYPE_HPP

namespace B33::System
{

typedef u32   EventId;
typedef usize EventAddr;

enum EEventHandleType
{
    Invalid,
    Funciton,
    Object,
};

} // namespace B33::System
#endif
