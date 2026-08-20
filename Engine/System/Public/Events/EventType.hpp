#if !defined( B33_EVENT_TYPE_HPP )
#    define B33_EVENT_TYPE_HPP

namespace B33::System
{

typedef uint32_t EventId;
typedef size_t   EventAddr;

enum EEventHandleType
{
    Invalid,
    Funciton,
    Object,
};

} // namespace B33::System

#endif
