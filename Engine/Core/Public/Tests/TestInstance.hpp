#if !defined( B3_TESTINSTANCE_HPP )
#    define B3_TESTINSTANCE_HPP

#    include "B33CoreMinimal.h"


#    if _B33_TESTS
#        define TEST( PTESTFUN )                                                                                       \
            inline void                             PTESTFUN##_TEST();                                                 \
            static ::B33::Core::Tests::TestInstance PTESTFUN##_TESTSTRCT =                                             \
                ::B33::Core::Tests::TestInstance( PTESTFUN##_TEST, __FILE__ ": " #PTESTFUN );                          \
            inline void PTESTFUN##_TEST()
#    else
#        define TEST( PTESTFUN )
#    endif // !_B33_TESTS

namespace B33::Core::Tests
{

struct TestInstance
{
    TestInstance( void ( *pTest )(), const char szName[] );
    ~TestInstance() = default;
};

} // namespace B33::Core::Tests
#endif
