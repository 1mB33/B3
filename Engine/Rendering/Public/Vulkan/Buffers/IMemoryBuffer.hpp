#if !defined( B33_IMEMORY_BUFFER_HPP )
#    define B33_IMEMORY_BUFFER_HPP

#    include <B33Core.h>

namespace B33::Rendering
{

template <class MEMORY>
class IMemoryBuffer
{
  public:
    IMemoryBuffer() noexcept  = default;
    ~IMemoryBuffer() noexcept = default;

  public:
    IMemoryBuffer( IMemoryBuffer && )                 = default;
    IMemoryBuffer &operator=( IMemoryBuffer && )      = default;
    IMemoryBuffer( const IMemoryBuffer & )            = default;
    IMemoryBuffer &operator=( const IMemoryBuffer & ) = default;

  public:
    inline void Free() noexcept
    {
        return static_cast<MEMORY *>( this )->FreeImpl();
    }
};

} // namespace B33::Rendering
#endif // !B33_IMEMORY_BUFFER_HPP
