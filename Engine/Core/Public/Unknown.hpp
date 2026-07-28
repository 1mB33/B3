#if !defined( B33_UNKNOWN_COLORS )
#    define B33_UNKNOWN_COLORS

#    include "B33CoreMinimal.h"

namespace B33::Core
{

typedef ::int64_t UnknownIndex;

__B33_API UnknownIndex IncreaseAndGetCounter() noexcept;

template <class T>
class Unknown
{
  public:
    Unknown()
    {
        if ( m_Index == 0 )
            m_Index = IncreaseAndGetCounter();
    }

    static UnknownIndex GetGlobalIndex()
    {
        return m_Index;
    }

    template<class DERIVED>
    static void Invoke()
    {
        __B33_ATTRIBUTE_MIGHT_BE_UNUSED DERIVED tmp;
    }


  private:
    static inline UnknownIndex m_Index = 0;
};

} // namespace B33::Core

#endif
