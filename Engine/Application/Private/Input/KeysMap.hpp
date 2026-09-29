#if !defined( B33_KEYS_MAP_H )
#    define B33_KEYS_MAP_H

#    include "IBindMap.hpp"
#    include "Input/KeyList.hpp"

namespace B33::App
{

class KeysMap : public IBindMap<KeysMap>
{
    static constexpr size_t AmountOfBindableKeys = B33_KEY_COUNT;

    template <typename T>
    using Vector = ::std::vector<T>;

    struct ActionReplayData
    {
        void     *This;
        B33Action Action;
    };

  public:
    KeysMap();

    explicit KeysMap( usize uAmountOfBindableKeys );

    ~KeysMap() noexcept = default;

  public:
    KeysMap( const KeysMap & )            = default;
    KeysMap &operator=( const KeysMap & ) = default;

    KeysMap( KeysMap && )            = default;
    KeysMap &operator=( KeysMap && ) = default;

  public:
    void BindActionImpl( const B33InputBind &ib, void *pThis, B33Action a, B33MouseAction ma );

    void UnbindActionImpl( const B33InputBind &ib, void *pThis );

  public:
    void PlayAction( const float fDelta, B33KeyId keyCode ) noexcept;

  private:
    Vector<ActionReplayData> m_vKeys;
};

} // namespace B33::App
#endif // !B33_KEYS_MAP_H
