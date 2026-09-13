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
    ~LinearAllocatorImpl() noexcept = default;

  public:
    LinearAllocatorImpl( LinearAllocatorImpl && )                 = default;
    LinearAllocatorImpl &operator=( LinearAllocatorImpl && )      = default;
    LinearAllocatorImpl( const LinearAllocatorImpl & )            = default;
    LinearAllocatorImpl &operator=( const LinearAllocatorImpl & ) = default;

  public:
    __B33_API void AllocImpl( usize uByteLength );
    __B33_API void FreeImpl( usize uByteLength );
    __B33_API void ResizeImpl( usize uBlockLength );
    __B33_API void ResetImpl() noexcept;

  private:
    void *m_pMemoryBlock  = nullptr;
    usize m_uMemoryLength = -1;
    usize m_uEndOffset    = -1;
};

class LinearAllocator
{
  public:
    LinearAllocator()           = default;
    ~LinearAllocator() noexcept = default;

  public:
    LinearAllocator( LinearAllocator && )                 = default;
    LinearAllocator &operator=( LinearAllocator && )      = default;
    LinearAllocator( const LinearAllocator & )            = default;
    LinearAllocator &operator=( const LinearAllocator & ) = default;

  private:
};

} // namespace B33::Core

#endif
