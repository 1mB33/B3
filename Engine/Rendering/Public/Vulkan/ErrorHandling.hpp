#if !defined( B33_ERROR_HANDLING_HPP )
#    define B33_ERROR_HANDLING_HPP

#    define THROW_IF_FAILED( result )                                                                                  \
        {                                                                                                              \
            if ( result != VK_SUCCESS )                                                                                \
            {                                                                                                          \
                B33_LOG( Core::Debug::Error,                                                                           \
                         L"ThrowIfFailed, vulkan isn't working!!! Error code is: %d",                                  \
                         static_cast<i32>( result ) );                                                                 \
                throw B33_EXCEPT( "Ohh nooo... Vulkan isn't working!!!" );                                             \
            }                                                                                                          \
        }

#endif // !B33_ERROR_HANDLING_HPP
