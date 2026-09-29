#include "B33Core.h"

#include "Unknown.hpp"

namespace B33::Core
{

static inline UnknownIndex g_Counter = 0;

UnknownIndex IncreaseAndGetCounter() noexcept
{
    return ++g_Counter;
}

} // namespace B33::Core
