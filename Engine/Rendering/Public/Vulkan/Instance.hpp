#if !defined( B33_INSTANCE_HPP )
#    define B33_INSTANCE_HPP

#    include "B33Rendering.h"

namespace B33::Rendering
{

class Instance
{
  public:
    __B33_API Instance();

    __B33_API ~Instance() noexcept;

  public:
    Instance( const Instance & ) noexcept            = delete;
    Instance &operator=( const Instance & ) noexcept = delete;

    Instance( Instance &&other ) noexcept;
    Instance &operator=( Instance &&other ) noexcept;

    // Getters // -----------------------------------------------------------------------------------------------------
  public:
    ::VkInstance GetInstance() const
    {
        return m_Instance;
    }

    // Internal // ----------------------------------------------------------------------------------------------------
  private:
    ::VkInstance CreateInstance();

  private:
    ::VkInstance m_Instance = VK_NULL_HANDLE;
#    ifdef _B33_DEBUG
    VkDebugUtilsMessengerEXT m_DebugExt = VK_NULL_HANDLE;
#    endif
};

} // namespace B33::Rendering
#endif // !B33_INSTANCE_HPP
