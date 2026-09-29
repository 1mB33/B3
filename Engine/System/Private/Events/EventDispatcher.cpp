#include "B33System.hpp"

#include "Events/EventDispatcher.hpp"

namespace B33::System
{

EventDispatcher::EventDispatcher()
  : m_pInstance( MakeShared<EventDispatcherInstance>() )
{
}

} // namespace B33::System
