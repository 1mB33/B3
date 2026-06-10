#if !defined( B33_UNKNOWN_COLORS )
#    define B33_UNKNOWN_COLORS

#include "B33CoreMinimal.h"

namespace B33::Core
{

typedef ::int64_t UnknownIndex;

__B33_API ::int64_t IncreaseAndGetCounter() noexcept;

template <class T>
class Unknown
{
  public:
    Unknown()
    {
        if ( m_Index == 0 )
            m_Index = IncreaseAndGetCounter();
    }

    static int GetGlobalIndex()
    {
        return m_Index;
    }

  private:
    static inline int m_Index = 0;
};

} // namespace B33::Core

#endif
