#include "B33Core.h"
#if !defined( B33_PAGES_TESTS_HPP )
#    define B33_PAGES_TESTS_HPP

#    include "Tests/TestInstance.hpp"
#    include "Memory/Pages.h"

TEST( Pages_ReserveAligned )
{
    const usize uRequesting = RequestPageSize() * 7;
    void       *pResult     = RequestPage( NULL, uRequesting );

    B33_INFO( L"Errno?=%d", errno );
    B33_ASSERT( !B33_PAGES_HAS_ERROR( pResult ) );
    B33_ASSERT( pResult != B33_PAGES_UNALIGNED_ERROR );
    DiscardPage( pResult, uRequesting );
}

TEST( Pages_ReserveMemAlignedButTooSmall )
{
    const usize uRequesting = RequestPageSize() > 1;
    void       *pResult     = RequestPage( NULL, uRequesting );
    B33_INFO( L"Errno?=%d", errno );
    B33_ASSERT( B33_PAGES_HAS_ERROR( pResult ) );
    B33_ASSERT( pResult == B33_PAGES_UNALIGNED_ERROR );
    DiscardPage( pResult, uRequesting );
}

TEST( Pages_ReserveUnaligned )
{
    const usize uRequesting = 3;
    void       *pResult     = RequestPage( NULL, uRequesting );
    B33_INFO( L"Errno?=%d", errno );
    B33_ASSERT( B33_PAGES_HAS_ERROR( pResult ) );
    B33_ASSERT( pResult == B33_PAGES_UNALIGNED_ERROR );
    DiscardPage( pResult, uRequesting );
}

#endif // !defined ( B33_PAGES_TESTS_HPP )
