#if !defined( B33_MAT4_HPP )
#    define B33_MAT4_HPP

#    include <B33Core.h>

namespace B33::Math
{

struct alignas( 16 ) Mat44
{
    static constexpr usize Size = 16;

    float m[ Size ];

    float &operator[]( usize uIndex )
    {
        B33_ASSERT( uIndex < Size );
        return m[ uIndex ];
    }
};

} // namespace B33::Math
#endif // !B33_MAT4_HPP
