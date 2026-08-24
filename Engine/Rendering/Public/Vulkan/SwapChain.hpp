#ifndef B33_SWAPCHAIN_H
#define B33_SWAPCHAIN_H

#include "B33Core.h"
#include "Instance.hpp"
#include "Window/WindowDesc.hpp"
#include "WrapperAdapter.hpp"
#include "WrapperHardware.hpp"

namespace B33::Rendering
{

class Swapchain
{
  public:
    static constexpr VkFormat TargetedFormat = VK_FORMAT_B8G8R8A8_UNORM;

  public:
    Swapchain() = default;

    __B33_API Swapchain( ::std::weak_ptr<const ::B33::Rendering::Instance>        inst,
                         ::std::weak_ptr<const ::B33::Rendering::HardwareWrapper> hw,
                         ::std::weak_ptr<const ::B33::Rendering::AdapterWrapper>  da,
                         ::std::weak_ptr<const ::WindowDesc>                      wd );

    __B33_API ~Swapchain();

  public:
    Swapchain( Swapchain && ) noexcept            = default;
    Swapchain &operator=( Swapchain && ) noexcept = delete;

    Swapchain( const Swapchain & ) noexcept            = delete;
    Swapchain &operator=( const Swapchain & ) noexcept = delete;

    // Getters // -----------------------------------------------------------------------------------------------------
  public:
    ::VkSwapchainKHR GetSwapChainHandle() const;

    ::VkImage GetImage( ::uint32_t i ) const;

    ::VkImage GetImage() const;

    ::VkImageView GetImageView() const;

    ::VkExtent2D GetExtent() const;

    ::uint32_t GetImageindex() const;

    ::uint32_t GetImageCount() const;

    // Setters // -----------------------------------------------------------------------------------------------------
  public:
    void SetCurrentImage( uint32_t uImageIndex )
    {
        B33_TRACE( L"Current swapchain image %d", uImageIndex );
        m_uCurrentImageIndex = uImageIndex;
    }

    // Internal // ----------------------------------------------------------------------------------------------------
  private:
    ::VkSurfaceKHR CreateSurface( ::std::weak_ptr<const ::B33::Rendering::Instance> &pInstance,
                                  ::std::weak_ptr<const ::WindowDesc>               &pWindowDesc );

    ::VkSurfaceCapabilitiesKHR
    GetCapabilitesInternal( ::std::weak_ptr<const ::B33::Rendering::HardwareWrapper> pHardware,
                            ::VkSurfaceKHR                                           surface );

    ::VkExtent2D GetExtentInternal( const VkSurfaceCapabilitiesKHR   &capabilities,
                                    ::std::weak_ptr<const WindowDesc> pWindowDesc );

    ::uint32_t GetImageCountInternal( const VkSurfaceCapabilitiesKHR &capabilities );

    ::VkSurfaceFormatKHR PickFormat( ::std::weak_ptr<const ::B33::Rendering::HardwareWrapper> &pHardware,
                                     VkSurfaceKHR                                              surface );

    ::VkPresentModeKHR PickPresentationMode( ::std::weak_ptr<const ::B33::Rendering::HardwareWrapper> &pHardware,
                                             VkSurfaceKHR                                              surface );

    ::VkSwapchainKHR CreateSwapChain( ::std::weak_ptr<const ::B33::Rendering::AdapterWrapper> &pAdapter,
                                      ::VkSurfaceKHR                                           surface,
                                      const ::VkSurfaceCapabilitiesKHR                        &capabilities,
                                      const ::VkExtent2D                                      &extent2D,
                                      ::uint32_t                                               uImageCount,
                                      const ::VkSurfaceFormatKHR                              &surfaceFormat,
                                      ::VkPresentModeKHR                                       presentMode );

    ::uint32_t GetNumberOfSwapChainImages( ::std::weak_ptr<const ::B33::Rendering::AdapterWrapper> &pAdapter,
                                           ::VkSwapchainKHR                                         swapchain );

    ::std::vector<::VkImage> CreateSwapChainImages( ::std::weak_ptr<const ::B33::Rendering::AdapterWrapper> &pAdapter,
                                                    ::VkSwapchainKHR                                         swapchain,
                                                    ::uint32_t                                               uAmount );

    ::std::vector<::VkImageView> CreateImageViews( ::std::weak_ptr<const ::B33::Rendering::AdapterWrapper> &pAdapter,
                                                   ::std::vector<::VkImage> swapChainImages,
                                                   ::uint32_t               uAmount );

  private:
    ::std::weak_ptr<const ::B33::Rendering::Instance>        m_pInstance      = {};
    ::std::weak_ptr<const ::B33::Rendering::HardwareWrapper> m_pHardware      = {};
    ::std::weak_ptr<const ::B33::Rendering::AdapterWrapper>  m_pDeviceAdapter = {};
    ::std::weak_ptr<const ::WindowDesc>                      m_pWindowDesc    = {};

    ::VkSurfaceKHR             m_Surface       = VK_NULL_HANDLE;
    ::VkSurfaceCapabilitiesKHR m_Capabilities  = {};
    ::VkExtent2D               m_Extent        = { 0, 0 };
    const ::uint32_t           m_uImageCount   = -1;
    ::VkSurfaceFormatKHR       m_SurfaceFormat = {};
    ::VkPresentModeKHR         m_PresentMode   = {};
    ::VkSwapchainKHR           m_pSwapChain    = VK_NULL_HANDLE;

    ::uint32_t                   m_uCurrentImageIndex = 0;
    ::std::vector<::VkImage>     m_SwapChainImages    = {};
    ::std::vector<::VkImageView> m_ImageViews {};
};

} // namespace B33::Rendering
#endif // !B33_SWAPCHAIN_H
