#if !defined( B33_PAGES_H )
#    define B33_PAGES_H

#    include "B33CoreMinimal.h"

#    if defined( _WIN32 )
#    elif defined( __linux__ ) || defined( __APPLE__ )
#        include <sys/mman.h>
#    endif

#    define B33_PAGES_NO_MEMORY_ERROR ( (void *)( -1 ) )
#    define B33_PAGES_UNALIGNED_ERROR ( (void *)( -2 ) )
#    define B33_PAGES_INTERNAL_ERROR  ( (void *)( -3 ) )

#    define B33_PAGES_ERROR          ( (void *)( -4 ) )
#    define B33_PAGES_HAS_ERROR( x ) ( (usize)( x ) >= (usize)( B33_PAGES_ERROR ) )

// RequestPageSize // -------------------------------------------------------------------------------------------------
inline usize RequestPageSize()
#    if defined( _WIN32 )
{
    return 0;
}
#    elif defined( __linux__ ) || defined( __APPLE__ )
{
    return getpagesize();
}
#    else
#        error "System not supported"
#    endif
// --------------------------------------------------------------------------------------------------------------------

// RequestPage // -----------------------------------------------------------------------------------------------------
inline void *RequestPage( void *pHint, usize uByteSize )
#    if defined( _WIN32 )
{
    return NULL;
}
#    elif defined( __linux__ ) || defined( __APPLE__ )
{
    void *pResult;
    int   flags = MAP_ANONYMOUS | MAP_PRIVATE;

#        if defined( _B33_DEBUG )
    if ( uByteSize & ( getpagesize() - 1 ) )
    {
        return B33_PAGES_UNALIGNED_ERROR;
    }
#        endif

    pResult = mmap( pHint, uByteSize, PROT_READ | PROT_WRITE, flags, -1, 0 );

    if ( pResult == MAP_FAILED )
    {
        // TODO: add some basic checks for errno
        return B33_PAGES_INTERNAL_ERROR;
    }

    return pResult;
}
#    else
#        error "System not supported"
#    endif
// --------------------------------------------------------------------------------------------------------------------

// DiscardPage // -----------------------------------------------------------------------------------------------------
inline usize DiscardPage( void *pPage, usize uByteSize )
#    if defined( _WIN32 )
{
}
#    elif defined( __linux__ ) || defined( __APPLE__ )
{
    int uResult;

    uResult = munmap( pPage, uByteSize );

    if ( uResult == -1 )
    {
        // TODO: add some basic checks for errno
        return (usize)B33_PAGES_INTERNAL_ERROR;
    }

    return 0;
}
#    else
#        error "System not supported"
#    endif
// --------------------------------------------------------------------------------------------------------------------

// LockPage // -----------------------------------------------------------------------------------------------------
inline usize LockPage( void *pPage, usize uByteSize )
#    if defined( _WIN32 )
{
}
#    elif defined( __linux__ ) || defined( __APPLE__ )
{
    int result;

    result = mlock( pPage, uByteSize );

    if ( result == -1 )
    {
        // TODO: add some basic checks for errno
        return (usize)B33_PAGES_INTERNAL_ERROR;
    }

    return 0;
}
#    else
#        error "System not supported"
#    endif
// --------------------------------------------------------------------------------------------------------------------

// UnlockPage // -----------------------------------------------------------------------------------------------------
inline usize UnlockPage( void *pPage, usize uByteSize )
#    if defined( _WIN32 )
{
}
#    elif defined( __linux__ ) || defined( __APPLE__ )
{
    int result;

    result = munlock( pPage, uByteSize );

    if ( result == -1 )
    {
        // TODO: add some basic checks for errno
        return (usize)B33_PAGES_INTERNAL_ERROR;
    }

    return 0;
}
#    else
#        error "System not supported"
#    endif
// --------------------------------------------------------------------------------------------------------------------
#endif
