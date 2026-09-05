#include "B33System.hpp"
#include "ComponentsOrder.hpp"
#include "EngineLoop.hpp"

namespace B33::System
{

using namespace std;
using namespace B33;

ComponentOrderRegister ComponentOrderRegister::RegisterOrder( Vector<StringView> order )
{
    EngineLoop::m_ComponentOrderRegistry = order;
    return ComponentOrderRegister();
}

} // namespace B33::System
