#if !defined( B33_BIND_MAP_H )
#    define B33_BIND_MAP_H

#    include "Input/Bind.h"

namespace B33::App
{

template <class MAP>
class IBindMap
{
  public:
    IBindMap() noexcept = default;

    ~IBindMap() noexcept = default;

  public:
    IBindMap( const IBindMap & ) noexcept            = default;
    IBindMap &operator=( const IBindMap & ) noexcept = default;

    IBindMap( IBindMap && ) noexcept            = default;
    IBindMap &operator=( IBindMap && ) noexcept = default;

  public:
    void BindAction( const B33InputBind &ib, void *pThis, B33Action a, B33MouseAction ma )
    {
        static_cast<MAP *>( this )->BindActionImpl( ib, pThis, a, ma );
    }

    void UnbindAction( const B33InputBind &ib, void *pThis )
    {
        static_cast<MAP *>( this )->UnbindActionImpl( ib, pThis );
    }
};

} // namespace B33::App
#endif // !B33_BIND_MAP_H
