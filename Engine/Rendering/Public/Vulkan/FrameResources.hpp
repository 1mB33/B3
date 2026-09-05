#if !defined( B33_FRAME_RESOURCES_HPP )
#    define B33_FRAME_RESOURCES_HPP

namespace B33::Rendering
{

struct Frame
{
    constexpr static usize MaxFramesInFlight = 2;

    ::VkFence         InFlightFence;
    ::VkSemaphore     ImageAvailable;
    ::VkCommandBuffer CommandBuffer;
};

} // namespace B33::Rendering
#endif // !B33_FRAME_RESOURCES_HPP
