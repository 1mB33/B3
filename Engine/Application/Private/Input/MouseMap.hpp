#if !defined( B33_MOUSE_MAP_H )
#    define B33_MOUSE_MAP_H

#    include "IBindMap.hpp"

namespace B33::App
{

class MouseMap : public IBindMap<MouseMap>
{
    template <typename T>
    using Vector = ::std::vector<T>;

    struct ActionReplayData
    {
        void          *pThis;
        B33MouseAction Action;
    };

  public:
    MouseMap() = default;

    ~MouseMap() noexcept = default;

  public:
    MouseMap( const MouseMap & )            = default;
    MouseMap &operator=( const MouseMap & ) = default;

    MouseMap( MouseMap && )            = default;
    MouseMap &operator=( MouseMap && ) = default;

  public:
    void BindActionImpl( const B33InputBind &ib, void *pThis, B33Action a, B33MouseAction ma );

    void UnbindActionImpl( const B33InputBind &ib, void *pThis );

  public:
    void PlayAction( const float fDelta, i32 fX, i32 fY ) noexcept;

  private:
    Vector<ActionReplayData> m_vMouseBinds;
};

} // namespace B33::App

#endif // !B33_MOUSE_MAP_H
