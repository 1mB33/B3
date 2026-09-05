#if !defined( B33_WRAPPER_HPP )
#    define B33_WRAPPER_HPP

namespace B33::Rendering
{

template <class Derived>
class IAdapter
{
    template <typename T>
    using Vector = ::std::vector<T>;

  public:
    IAdapter() noexcept = default;

    ~IAdapter() noexcept = default;

  public:
    IAdapter( IAdapter && ) noexcept      = default;
    IAdapter( const IAdapter & ) noexcept = default;

    IAdapter &operator=( IAdapter && ) noexcept      = default;
    IAdapter &operator=( const IAdapter & ) noexcept = default;

    // Intreface // ---------------------------------------------------------------------------------------------------
  public:
    const Vector<const char *> &GetExtensions() const
    {
        return static_cast<const Derived *>( this )->GetExtensionsImpl();
    }

    void *GetFeatures() const
    {
        return static_cast<const Derived *>( this )->GetFeaturesImpl();
    }

    u32 GetQueueFlags() const
    {
        return static_cast<const Derived *>( this )->GetQueueFlagsImpl();
    }
};

} // namespace B33::Rendering
#endif // !B33_WRAPPER_HPP
