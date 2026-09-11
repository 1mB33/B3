#include "B33Core.h"

#include "Memory/Pages.h"

#if defined( _WIN32 )
#elif defined( __linux__ ) || defined( __APPLE__ )
#    include <sys/mman.h>
#endif

// --------------------------------------------------------------------------------------------------------------------
usize RequestPageSize()
#if defined( _WIN32 )
{
    return 0;
}
#elif defined( __linux__ ) || defined( __APPLE__ )
{
    return getpagesize();
}
#else
#    error "System not supported"
#endif

// --------------------------------------------------------------------------------------------------------------------
void *RequestPage( void *pHint, usize uByteSize )
#if defined( _WIN32 )
{
    return NULL;
}
#elif defined( __linux__ ) || defined( __APPLE__ )
{
    void *pResult = NULL;
    int   flags   = MAP_ANONYMOUS | MAP_PRIVATE;

#    if defined( _B33_DEBUG )
    if ( uByteSize & ( getpagesize() - 1 ) )
    {
        return B33_PAGES_UNALIGNED_ERROR;
    }
#    endif

    pResult = mmap( pHint, uByteSize, PROT_READ | PROT_WRITE, flags, -1, 0 );

    if ( pResult == MAP_FAILED )
    {
        return B33_PAGES_INTERNAL_ERROR;
    }

    return pResult;
}
#else
#    error "System not supported"
#endif

// --------------------------------------------------------------------------------------------------------------------
void DiscardPage( void *pPage, usize uByteSize )
#if defined( _WIN32 )
{
}
#elif defined( __linux__ ) || defined( __APPLE__ )
{
    munmap( pPage, uByteSize );
}
#else
#    error "System not supported"
#endif
