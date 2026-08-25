#include "B33Rendering.hpp"

#include "Vulkan/Instance.hpp"

namespace B33::Rendering
{

using namespace std;

// Instance // ---------------------------------------------------------------------------------------------------------
Instance::Instance()
  : m_Instance( CreateInstance() )
{
    B33_LOG( B33::Core::Debug::Info, L"Creating an instance!" );
}

// ---------------------------------------------------------------------------------------------------------------------
Instance::~Instance()
{
    if ( m_Instance != VK_NULL_HANDLE )
    {
        B33_LOG( B33::Core::Debug::Info, L"Destroying instance" );
        vkDestroyInstance( m_Instance, NULL );
        m_Instance = VK_NULL_HANDLE;
    }
}

// --------------------------------------------------------------------------------------------------------------------
Instance::Instance( Instance &&other ) noexcept
  : m_Instance( other.m_Instance )
{
    other.m_Instance = VK_NULL_HANDLE;
}

// --------------------------------------------------------------------------------------------------------------------
Instance &Instance::operator=( Instance &&other ) noexcept
{
    this->m_Instance = other.m_Instance;
    other.m_Instance = VK_NULL_HANDLE;
    return *this;
}

// ---------------------------------------------------------------------------------------------------------------------
__B33_ATTRIBUTE_MIGHT_BE_UNUSED static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback( __B33_ATTRIBUTE_MIGHT_BE_UNUSED VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
               __B33_ATTRIBUTE_MIGHT_BE_UNUSED VkDebugUtilsMessageTypeFlagsEXT        messageType,
               const VkDebugUtilsMessengerCallbackDataEXT                            *pCallbackData,
               __B33_ATTRIBUTE_MIGHT_BE_UNUSED void                                  *pUserData )
{
    constexpr wchar_t pwszFormat[] =
#if defined( _WIN32 )
        L"%S";
#elif defined( __linux__ ) || defined( __APPLE__ )
        L"Message: %s";
#endif // !_WIN32

    ::B33::Core::Debug::Logger::Get().Log( "Vulkan", Core::Debug::Info, pwszFormat, pCallbackData->pMessage );
    return VK_TRUE;
}

// Private // ----------------------------------------------------------------------------------------------------------
VkInstance Instance::CreateInstance()
{
    VkInstance instance;
    VkResult   result;

#ifdef _B33_DEBUG
    vector<VkValidationFeatureEnableEXT> enabledVaditationFeatures = {
        VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT,
        VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT,
        //VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT,
        VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT,
        //VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT,
    };

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
    debugCreateInfo.sType                              = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugCreateInfo.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugCreateInfo.pfnUserCallback = debugCallback;

    VkValidationFeaturesEXT validationFeatures       = {};
    validationFeatures.sType                         = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
    validationFeatures.pNext                         = &debugCreateInfo;
    validationFeatures.enabledValidationFeatureCount = static_cast<uint32_t>( enabledVaditationFeatures.size() );
    validationFeatures.pEnabledValidationFeatures    = &enabledVaditationFeatures[ 0 ];

#endif // !_B33_DEBUG

    const vector<const char *> vpszValidationLayers = {
// This layer is present in Lunar SDK for windows and linux, but doesn't seem to be working on apple.
// I don't have energy to deal with it rigth now
#if defined( _B33_DEBUG ) && ( defined( _WIN32 ) || defined( __linux__ ) )
        "VK_LAYER_KHRONOS_validation",
#endif // !_B33_DEBUG
    };

    const vector<const char *> vpszExtensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
#if defined( _WIN32 )
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif defined( _X11 )
        VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
#elif defined( __APPLE__ )
        VK_EXT_METAL_SURFACE_EXTENSION_NAME,
#endif // !_WIN32

#ifdef _B33_DEBUG
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
    };

    VkApplicationInfo appInfo  = {};
    appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName   = "B33::Rendering";
    appInfo.applicationVersion = VK_MAKE_VERSION( 0, 1, 5 );
    appInfo.pEngineName        = "AtlanticBeast";
    appInfo.engineVersion      = VK_MAKE_VERSION( 0, 2, 0 );
    appInfo.apiVersion         = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType                = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    createInfo.pNext =
#ifdef _B33_DEBUG
        &validationFeatures,
#else
        NULL,
#endif
    createInfo.pApplicationInfo        = &appInfo;
    createInfo.enabledLayerCount       = static_cast<uint32_t>( vpszValidationLayers.size() );
    createInfo.ppEnabledLayerNames     = !vpszValidationLayers.empty() ? &vpszValidationLayers[ 0 ] : nullptr;
    createInfo.enabledExtensionCount   = static_cast<uint32_t>( vpszExtensions.size() );
    createInfo.ppEnabledExtensionNames = !vpszExtensions.empty() ? &vpszExtensions[ 0 ] : nullptr;

    result = vkCreateInstance( &createInfo, NULL, &instance );

    if ( result != VK_SUCCESS )
    {
        B33_LOG( B33::Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!! Error code is: %d", result );
        throw B33_EXCEPT( "Ohh nooo... Vulkan isn't working!!!" );
    }

#ifdef _B33_DEBUG
    PFN_vkCreateDebugUtilsMessengerEXT myvkCreateDebugUtilsMessengerEXT =
        reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr( instance, "vkCreateDebugUtilsMessengerEXT" ) );

    myvkCreateDebugUtilsMessengerEXT( instance, &debugCreateInfo, nullptr, &m_DebugExt );
#endif

    return instance;
}

} // namespace B33::Rendering
