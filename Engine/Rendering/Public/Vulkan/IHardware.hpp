#if !defined( B33_IHARDWARE_HPP )
#    define B33_IHARDWARE_HPP

#    include "Instance.hpp"

namespace B33::Rendering
{

template <class DERIVED>
class IHardware
{
  public:
    IHardware() noexcept = default;

    ~IHardware() noexcept = default;

  public:
    IHardware( IHardware && ) noexcept            = default;
    IHardware &operator=( IHardware && ) noexcept = default;

    IHardware( const IHardware & ) noexcept            = default;
    IHardware &operator=( const IHardware & ) noexcept = default;

    // Interface // ---------------------------------------------------------------------------------------------------
  public:
    ::VkPhysicalDevice ChooseHardware( VkInstance Instance ) const
    {
        return static_cast<const DERIVED *>( this )->ChooseHardwareImpl( Instance );
    }
};

} // namespace B33::Rendering

#endif // !B33_IHARDWARE_HPP
