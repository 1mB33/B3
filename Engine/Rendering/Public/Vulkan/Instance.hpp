#ifndef B33_INSTANCE_H
#define B33_INSTANCE_H

#include "B33Rendering.hpp"

namespace B33::Rendering
{

class Instance
{
  public:
    __B33_API Instance();

    __B33_API ~Instance();

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
};

} // namespace B33::Rendering
#endif // !B33_INSTANCE_H
