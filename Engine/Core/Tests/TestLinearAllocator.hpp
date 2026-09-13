#if !defined( B33_LINEAR_ALLOCATOR_TESTS_HPP )
#    define B33_LINEAR_ALLOCATOR_TESTS_HPP

#    include "Memory/LinearAllocator.hpp"
#    include "Tests/TestInstance.hpp"

TEST( LinearAllocator_ImplConstructorCorrect )
{
    B33::Core::LinearAllocatorImpl impl( RequestPageSize() );
    impl.AllocImpl( 32 );
}

#endif // !defined ( B33_LINEAR_ALLOCATOR_TESTS_HPP )
