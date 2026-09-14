#if !defined( B33_LINEAR_ALLOCATOR_HPP )
#    define B33_LINEAR_ALLOCATOR_HPP

#    include "Pages.h"
#    include "IAllocatorImpl.hpp"

namespace B33::Core
{

class LinearAllocatorImpl : public IAllocatorImpl<LinearAllocatorImpl>
{
  public:
    LinearAllocatorImpl() = delete;
    explicit LinearAllocatorImpl( usize uByteBlockSize );
    ~LinearAllocatorImpl() noexcept;

  public:
    LinearAllocatorImpl( LinearAllocatorImpl && )                 = default;
    LinearAllocatorImpl &operator=( LinearAllocatorImpl && )      = default;
    LinearAllocatorImpl( const LinearAllocatorImpl & )            = delete;
    LinearAllocatorImpl &operator=( const LinearAllocatorImpl & ) = delete;

  public:
    __B33_API usize GetUsedLengthImpl() const;

    __B33_API usize GetAllocatedLengthImpl() const;

  public:
    __B33_API void *AllocImpl( usize uByteLength, usize uAlignment );
    __B33_API void  FreeImpl( void *pMemory, usize uByteLength );
    __B33_API void  ResetImpl() noexcept;

  private:
    void *m_pMemoryBlock  = nullptr;
    usize m_uMemoryLength = -1;
    usize m_uEndOffset    = -1;
};

template <usize POOL_SIZE>
class LinearAllocator : public LinearAllocatorImpl
{
  public:
    LinearAllocator( const char *pszName = nullptr )
      : LinearAllocatorImpl( POOL_SIZE )
#    if defined( _B33_DEBUG )
      , m_pszName( pszName )
#    endif
    {
    }

    ~LinearAllocator() noexcept = default;

  public:
    LinearAllocator( LinearAllocator && )                 = default;
    LinearAllocator &operator=( LinearAllocator && )      = default;
    LinearAllocator( const LinearAllocator & )            = delete;
    LinearAllocator &operator=( const LinearAllocator & ) = delete;

  public:
    inline const char *GetName() const
    {
#    if defined( _B33_DEBUG )
        return m_pszName;
#    endif
        return nullptr;
    }

    // Setters // -----------------------------------------------------------------------------------------------------
  public:
    inline void SetName( const char *pszName )
    {
#    if defined( _B33_DEBUG )
        m_pszName = pszName;
#    endif
    }

  private:
#    if defined( _B33_DEBUG )
    const char *m_pszName = nullptr;
#    endif
};

} // namespace B33::Core

#endif
