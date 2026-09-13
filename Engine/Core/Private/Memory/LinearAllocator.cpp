#include "B33Core.h"

#include "Memory/LinearAllocator.hpp"
#include "Memory/Pages.h"

namespace B33::Core
{

LinearAllocatorImpl::LinearAllocatorImpl( usize uByteBlockSize )
  : m_pMemoryBlock( nullptr )
  , m_uMemoryLength( 0 )
  , m_uEndOffset( 0 )
{
    void *pRequestedPage;

    pRequestedPage = RequestPage( nullptr, uByteBlockSize );

    if ( B33_PAGES_HAS_ERROR( pRequestedPage ) )
    {
        throw B33_EXCEPT_BAD_ARGUMENT();
    }

    m_pMemoryBlock  = pRequestedPage;
    m_uMemoryLength = uByteBlockSize;
}

void LinearAllocatorImpl::AllocImpl( usize uByteLength )
{
    const usize uNewLenght = m_uEndOffset + uByteLength;
    usize       uLockResult;

    if ( uNewLenght > m_uMemoryLength )
    {
        throw B33_EXCEPT_BAD_ARGUMENT();
    }

    uLockResult = LockPage( m_pMemoryBlock, uNewLenght );
    if ( B33_PAGES_HAS_ERROR( uLockResult ) )
    {
        B33_ERROR( L"Error on lock %d, errno %d", uLockResult, errno );
        throw B33_EXCEPT_NO_MORE_MEMORY();
    }

    m_uEndOffset = uNewLenght;
}

void LinearAllocatorImpl::FreeImpl( usize ) {}

void LinearAllocatorImpl::ResizeImpl( usize ) {}

void LinearAllocatorImpl::ResetImpl() noexcept {}

} // namespace B33::Core
