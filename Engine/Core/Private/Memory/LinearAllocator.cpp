#include "B33Core.h"

#include "Memory/LinearAllocator.hpp"
#include "Memory/Pages.h"

namespace B33::Core
{

// --------------------------------------------------------------------------------------------------------------------
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

// --------------------------------------------------------------------------------------------------------------------
LinearAllocatorImpl::~LinearAllocatorImpl() noexcept
{
    Reset();
}

// --------------------------------------------------------------------------------------------------------------------
usize LinearAllocatorImpl::GetUsedLengthImpl() const
{
    return m_uEndOffset;
}

// --------------------------------------------------------------------------------------------------------------------
usize LinearAllocatorImpl::GetAllocatedLengthImpl() const
{
    return m_uMemoryLength;
}

// --------------------------------------------------------------------------------------------------------------------
void *LinearAllocatorImpl::AllocImpl( usize uByteLength, usize uAlignment )
{
    const usize uPadding           = ( uAlignment - 1 ) - ( ( uByteLength + ( uAlignment - 1 ) ) & ( uAlignment - 1 ) );
    const usize uAlignedByteLength = uByteLength + uPadding;
    const usize uNewLenght         = m_uEndOffset + uAlignedByteLength;
    usize       uLockResult;
    void       *pResult;

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

    pResult      = reinterpret_cast<void *>( reinterpret_cast<usize>( m_pMemoryBlock ) + m_uEndOffset );
    m_uEndOffset = uNewLenght;
    return pResult;
}

// --------------------------------------------------------------------------------------------------------------------
void LinearAllocatorImpl::FreeImpl( void *, usize ) {}

// --------------------------------------------------------------------------------------------------------------------
void LinearAllocatorImpl::ResetImpl() noexcept
{
    usize uUnlockResult;
    usize uReleasePageResult;

    uUnlockResult = UnlockPage( m_pMemoryBlock, m_uMemoryLength );
    if ( B33_PAGES_HAS_ERROR( uUnlockResult ) )
    {
        B33_ERROR( L"Error on unlock %d, errno %d", uUnlockResult, errno );
    }

    uReleasePageResult = DiscardPage( m_pMemoryBlock, m_uMemoryLength );
    if ( B33_PAGES_HAS_ERROR( uReleasePageResult ) )
    {
        B33_ERROR( L"Error on discard %d, errno %d", uReleasePageResult, errno );
    }
}

} // namespace B33::Core
