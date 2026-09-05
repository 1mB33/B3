#if !defined( B33_EVENT_HPP )
#    define B33_EVENT_HPP

#    include <Unknown.hpp>

namespace B33::System
{

template <class EVENT_NAME>
class Event : public B33::Core::Unknown<EVENT_NAME>
{
};

} // namespace B33::System

#endif
