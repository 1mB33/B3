#include "B33System.hpp"
#include "IComponent.hpp"
#include "EngineLoop.hpp"

namespace B33::System
{

using namespace std;
using namespace B33;

void ComponentInstanceRegister::RegisterInternal( const StringView &className, ComponentFactory factory )
{
    EngineLoop::m_ComponentRegistry[ className ] = factory;
}

} // namespace B33::System
