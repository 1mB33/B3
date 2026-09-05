#if !defined( B33_WRAPPER_HARDWARE_HPP )
#    define B33_WRAPPER_HARDWARE_HPP

#    include "Vulkan/Instance.hpp"

namespace B33::Rendering
{

class HardwareWrapper
{
    template <typename T>
    using SharedPtr = ::std::shared_ptr<T>;

  public:
    HardwareWrapper()
      : m_PhysicalDevice( nullptr )
    {
    }

    ~HardwareWrapper() noexcept
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
    void Initialize( SharedPtr<const Instance> pInstance, const T &hardware )
    {
        B33_LOG( Core::Debug::Info, L"Initializing hardware" );
        m_pInstance = pInstance;

        m_PhysicalDevice = hardware.ChooseHardware( m_pInstance->GetInstance() );
    }

    // Internal // ----------------------------------------------------------------------------------------------------
  private:
    SharedPtr<const Instance> m_pInstance = nullptr;

    VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
};

} // namespace B33::Rendering
#endif // !B33_WRAPPER_HARDWARE_HPP
