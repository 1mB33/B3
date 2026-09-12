#if !defined( B33_LINEAR_ALLOCATOR_HPP )
#    define B33_LINEAR_ALLOCATOR_HPP

#include "Pages.h"

namespace B33::Core
{

class LinearAllocator
{
  public:
    LinearAllocator()  = default;
    ~LinearAllocator() = default;

  public:
    LinearAllocator( LinearAllocator && )                 = default;
    LinearAllocator &operator=( LinearAllocator && )      = default;
    LinearAllocator( const LinearAllocator & )            = default;
    LinearAllocator &operator=( const LinearAllocator & ) = default;

  private:
};

} // namespace B33::Core

#endif
