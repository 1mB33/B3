#if !defined( B33_MAT4_HPP )
#    define B33_MAT4_HPP

#    if defined( _B33_SSE )
#        include <xmmintrin.h>
#        include <tmmintrin.h>
#    endif
#    include <B33Core.h>

namespace B33::Math
{

struct alignas( 16 ) Mat44
{
    static constexpr usize Size = 16;

    union
    {
        float m[ Size ];

        struct
        {
            __m128 row[ Size / 4 ];
        };
    };

    float &operator[]( usize uIndex )
    {
        B33_ASSERT( uIndex < Size );
        return m[ uIndex ];
    }
};

} // namespace B33::Math
#endif // !B33_MAT4_HPP
