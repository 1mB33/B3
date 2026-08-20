#ifndef B33_WRAPPER_HARDWARE_H
#define B33_WRAPPER_HARDWARE_H

#include "Vulkan/Instance.hpp"

namespace B33::Rendering
{

class HardwareWrapper
{
  public:
    HardwareWrapper()
      : m_PhysicalDevice( nullptr )
    {
    }

    ~HardwareWrapper()
    {
        B33_LOG( Core::Debug::Info, L"Destroying hardware" );
    }

  public:
    HardwareWrapper( HardwareWrapper && ) noexcept            = default;
    HardwareWrapper &operator=( HardwareWrapper && ) noexcept = default;

    HardwareWrapper( const HardwareWrapper & )            = delete;
    HardwareWrapper &operator=( const HardwareWrapper & ) = delete;

    // Getters // -----------------------------------------------------------------------------------------------------
  public:
    VkPhysicalDevice GetPhysicalDevice() const
    {
        return m_PhysicalDevice;
    }

    // Methods // -----------------------------------------------------------------------------------------------------
  public:
    template <class T>
    void Initialize( ::std::weak_ptr<const ::B33::Rendering::Instance> pInstance, const T &hardware )
    {
        B33_LOG( Core::Debug::Info, L"Initializing hardware" );
        auto pLockedInstance = pInstance.lock();
        if ( !pLockedInstance )
            throw B33_EXCEPT( "Instance cannot be locked on hardware initialization" );

        m_PhysicalDevice = hardware.ChooseHardware( pLockedInstance->GetInstance() );
    }

    // Internal // ----------------------------------------------------------------------------------------------------
  private:
    VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
};

} // namespace B33::Rendering

#endif // !B33_WRAPPER_HARDWARE_H
