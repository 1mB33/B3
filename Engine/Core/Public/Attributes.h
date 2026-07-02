#if defined( _GNU )
#    define __B33_ATTRIBUTE_MIGHT_BE_UNUSED __attribute__( ( unused ) )
#elif defined( _MSVC )
#    define __B33_ATTRIBUTE_MIGHT_BE_UNUSED
#elif defined( _CLANG )
#    define __B33_ATTRIBUTE_MIGHT_BE_UNUSED __attribute__( ( unused ) )
#else
#    error "Compiler isn't supported"
#endif
