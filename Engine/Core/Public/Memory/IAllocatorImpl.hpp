#if !defined( B33_IALLOCATOR_IMPL_HPP )
#    define B33_IALLOCATOR_IMPL_HPP

namespace B33::Core
{

template <class ALLOCATOR>
class IAllocatorImpl
{
  public:
    IAllocatorImpl()           = default;
    ~IAllocatorImpl() noexcept = default;

  public:
    IAllocatorImpl( IAllocatorImpl && )                 = default;
    IAllocatorImpl &operator=( IAllocatorImpl && )      = default;
    IAllocatorImpl( const IAllocatorImpl & )            = default;
    IAllocatorImpl &operator=( const IAllocatorImpl & ) = default;

  public:
    inline usize GetUsedLength() const
    {
        return static_cast<const ALLOCATOR *>( this )->GetUsedLengthImpl();
    }

    inline usize GetAllocatedLength() const
    {
        return static_cast<const ALLOCATOR *>( this )->GetAllocatedLengthImpl();
    }

  public:
    inline void *Alloc( usize uByteLength, usize uAlignment )
    {
        return static_cast<ALLOCATOR *>( this )->AllocImpl( uByteLength, uAlignment );
    }

    inline void Free( void *pMemory, usize uByteLength )
    {
        static_cast<ALLOCATOR *>( this )->FreeImpl( pMemory, uByteLength );
    }

    inline void Reset() noexcept
    {
        static_cast<ALLOCATOR *>( this )->ResetImpl();
    }
};

} // namespace B33::Core

#endif
