#if !defined( B33_PAGES_H )
#    define B33_PAGES_H

#    include "B33CoreMinimal.h"

#    define B33_PAGES_NO_MEMORY_ERROR ( (void *)( -1 ) )
#    define B33_PAGES_UNALIGNED_ERROR ( (void *)( -2 ) )
#    define B33_PAGES_INTERNAL_ERROR  ( (void *)( -3 ) )

#    define B33_PAGES_ERROR          ( (void *)( -4 ) )
#    define B33_PAGES_HAS_ERROR( x ) ( (usize)( x ) >= (usize)( B33_PAGES_ERROR ) )

// RequestPageSize // -------------------------------------------------------------------------------------------------
#    if defined( __cplusplus )
extern "C"
{
#    endif
    __B33_API usize RequestPageSize();
#    if defined( __cplusplus )
}
#    endif
// --------------------------------------------------------------------------------------------------------------------

// RequestPage // -----------------------------------------------------------------------------------------------------
#    if defined( __cplusplus )
extern "C"
{
#    endif
    __B33_API void *RequestPage( void *pHint, usize uByteSize );
#    if defined( __cplusplus )
}
#    endif
// --------------------------------------------------------------------------------------------------------------------

// DiscardPage // -----------------------------------------------------------------------------------------------------
#    if defined( __cplusplus )
extern "C"
{
#    endif
    __B33_API void DiscardPage( void *pPage, usize uByteSize );
#    if defined( __cplusplus )
}
#    endif
// --------------------------------------------------------------------------------------------------------------------

#endif
