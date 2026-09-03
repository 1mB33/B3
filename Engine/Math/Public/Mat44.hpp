#if !defined(B33_MAT4_H)
#define B33_MAT4_H

namespace B33::Math
{

struct alignas( 16 ) Mat44
{
    static constexpr size_t Size = 16;

    float m[ Size ];

    float &operator[]( size_t uIndex )
    {
        B33_ASSERT(uIndex < Size);
        return m[ uIndex ];
    }
};

} // namespace B33::Math
#endif // !B33_MAT4_H
