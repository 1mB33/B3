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
    inline void Alloc( usize uByteLength )
    {
        static_cast<ALLOCATOR *>( this )->AllocImpl( uByteLength );
    }

    inline void Free( usize uByteLength )
    {
        static_cast<ALLOCATOR *>( this )->FreeImpl( uByteLength );
    }

    inline void Resize( usize uByteLength )
    {
        static_cast<ALLOCATOR *>( this )->ResizeImpl( uByteLength );
    }

    inline void Reset() noexcept
    {
        static_cast<ALLOCATOR *>( this )->ResetImpl();
    }
};

} // namespace B33::Core

#endif
