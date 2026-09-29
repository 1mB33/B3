#if !defined( B33_SWAPCHAIN_HPP )
#    define B33_SWAPCHAIN_HPP

#    include <B33Core.h>
#    include "Instance.hpp"
#    include "Window/WindowDesc.hpp"
#    include "WrapperAdapter.hpp"
#    include "WrapperHardware.hpp"

namespace B33::Rendering
{

class Swapchain
{
    template <typename T>
    using WeakPtr = ::std::weak_ptr<T>;
    template <typename T>
    using Vector          = ::std::vector<T>;
    using Instance        = ::B33::Rendering::Instance;
    using HardwareWrapper = ::B33::Rendering::HardwareWrapper;
    using AdapterWrapper  = ::B33::Rendering::AdapterWrapper;

  public:
    static constexpr VkFormat TargetedFormat = VK_FORMAT_B8G8R8A8_UNORM;

  public:
    Swapchain() = default;

    __B33_API Swapchain( WeakPtr<const Instance>        inst,
                         WeakPtr<const HardwareWrapper> hw,
                         WeakPtr<const AdapterWrapper>  da,
                         const ::WindowDesc            *wd );

    __B33_API ~Swapchain() noexcept;

  public:
    Swapchain( Swapchain && ) noexcept            = default;
    Swapchain &operator=( Swapchain && ) noexcept = delete;

    Swapchain( const Swapchain & ) noexcept            = delete;
    Swapchain &operator=( const Swapchain & ) noexcept = delete;

    // Getters // -----------------------------------------------------------------------------------------------------
  public:
    ::VkSwapchainKHR GetSwapChainHandle() const;

    ::VkImage GetImage( u32 i ) const;

    ::VkImage GetCurrentImage() const;

    ::VkImageView GetCurrentImageView() const;

    ::VkExtent2D GetExtent() const;

    ::u32 GetCurrentImageIndex() const;

    ::u32 GetImageCount() const;

    // Setters // -----------------------------------------------------------------------------------------------------
  public:
    void SetCurrentImage( u32 uImageIndex )
    {
        B33_TRACE( L"Current swapchain image %d", uImageIndex );
        m_uCurrentImageIndex = uImageIndex;
    }

    // Internal // ----------------------------------------------------------------------------------------------------
  private:
    ::VkSurfaceKHR CreateSurface( WeakPtr<const Instance> &pInstance, const WindowDesc *pWindowDesc );

    ::VkSurfaceCapabilitiesKHR GetCapabilitesInternal( WeakPtr<const HardwareWrapper> pHardware,
                                                       ::VkSurfaceKHR                 surface );

    ::VkExtent2D GetExtentInternal( const VkSurfaceCapabilitiesKHR &capabilities, const WindowDesc *pWindowDesc );

    ::u32 GetImageCountInternal( const VkSurfaceCapabilitiesKHR &capabilities );

    ::VkSurfaceFormatKHR PickFormat( WeakPtr<const HardwareWrapper> &pHardware, VkSurfaceKHR surface );

    ::VkPresentModeKHR PickPresentationMode( WeakPtr<const HardwareWrapper> &pHardware, VkSurfaceKHR surface );

    ::VkSwapchainKHR CreateSwapChain( WeakPtr<const AdapterWrapper>    &pAdapter,
                                      ::VkSurfaceKHR                    surface,
                                      const ::VkSurfaceCapabilitiesKHR &capabilities,
                                      const ::VkExtent2D               &extent2D,
                                      u32                               uImageCount,
                                      const ::VkSurfaceFormatKHR       &surfaceFormat,
                                      ::VkPresentModeKHR                presentMode );

    ::u32 GetNumberOfSwapChainImages( WeakPtr<const AdapterWrapper> &pAdapter, ::VkSwapchainKHR swapchain );

    Vector<::VkImage>
    CreateSwapChainImages( WeakPtr<const AdapterWrapper> &pAdapter, ::VkSwapchainKHR swapchain, u32 uAmount );

    Vector<::VkImageView> CreateImageViews( WeakPtr<const AdapterWrapper> &pAdapter,
                                            Vector<::VkImage>              swapChainImages,
                                            VkFormat                       format,
                                            u32                            uAmount );

  private:
    WeakPtr<const Instance>        m_pInstance      = {};
    WeakPtr<const HardwareWrapper> m_pHardware      = {};
    WeakPtr<const AdapterWrapper>  m_pDeviceAdapter = {};
    const WindowDesc              *m_pWindowDesc    = {};

    ::VkSurfaceKHR             m_Surface       = VK_NULL_HANDLE;
    ::VkSurfaceCapabilitiesKHR m_Capabilities  = {};
    ::VkExtent2D               m_Extent        = { 0, 0 };
    const u32                  m_uImageCount   = -1;
    ::VkSurfaceFormatKHR       m_SurfaceFormat = {};
    ::VkPresentModeKHR         m_PresentMode   = {};
    ::VkSwapchainKHR           m_pSwapChain    = VK_NULL_HANDLE;

    u32                   m_uCurrentImageIndex = 0;
    Vector<::VkImage>     m_SwapChainImages    = {};
    Vector<::VkImageView> m_ImageViews {};
};

} // namespace B33::Rendering
#endif // !B33_SWAPCHAIN_HPP
