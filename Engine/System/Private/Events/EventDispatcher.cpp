#include "B33System.hpp"
#include "Events/EventDispatcher.hpp"

namespace B33::System
{

EventDispatcher::EventDispatcher()
  : m_pInstance( ::std::make_shared<EventDispatcherInstance>() )
{
}

} // namespace B33::System
