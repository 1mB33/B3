#if !defined( B33_UNKNOWN_COLORS_HPP )
#    define B33_UNKNOWN_COLORS_HPP

#    include "B33CoreMinimal.h"
#    include "Attributes.h"

namespace B33::Core
{

typedef i64 UnknownIndex;

__B33_API UnknownIndex IncreaseAndGetCounter() noexcept;

template <class T>
class Unknown
{
  public:
    Unknown() noexcept
    {
        if ( m_Index == 0 )
            m_Index = IncreaseAndGetCounter();
    }

    static UnknownIndex GetGlobalIndex() noexcept
    {
        return m_Index;
    }

    template <class DERIVED>
    static void Invoke()
    {
        __B33_ATTRIBUTE_MIGHT_BE_UNUSED DERIVED tmp;
    }


  private:
    static inline UnknownIndex m_Index = 0;
};

} // namespace B33::Core

#endif
