#include "B33Core.h"

#include "Unknown.hpp"

namespace B33::Core
{

static inline int g_Counter = 0;

::int64_t IncreaseAndGetCounter() noexcept
{
    return ++g_Counter;
}

}
