#if !defined( B33_LINEAR_ALLOCATOR_TESTS_HPP )
#    define B33_LINEAR_ALLOCATOR_TESTS_HPP

#    include "Memory/LinearAllocator.hpp"
#    include "Tests/TestInstance.hpp"

TEST( LinearAllocator_ImplConstructorCorrect )
{
    B33::Core::LinearAllocatorImpl impl( RequestPageSize() );
}

TEST( LinearAllocator_ImplAllocCorrect )
{
    B33::Core::LinearAllocatorImpl impl( RequestPageSize() );
    impl.AllocImpl( 55, sizeof( usize ) );
    B33_INFO( L"Allocated %d Area %d", impl.GetUsedLength(), impl.GetAllocatedLength() );
}

TEST( LinearAllocator_ImplDeallocCorrect )
{
    B33::Core::LinearAllocatorImpl impl( RequestPageSize() );
    auto                           p = impl.AllocImpl( 64, sizeof( usize ) );
    impl.FreeImpl( p, 33 );
    B33_INFO( L"Allocated %d Area %d", impl.GetUsedLength(), impl.GetAllocatedLength() );
}

#endif // !defined ( B33_LINEAR_ALLOCATOR_TESTS_HPP )
