#if !defined(B33_ATTRIBUTES_H)
#define B33_ATTRIBUTES_H

#if defined( _GNU )
#    define __B33_ATTRIBUTE_MIGHT_BE_UNUSED __attribute__( ( unused ) )
#elif defined( _MSVC )
#    define __B33_ATTRIBUTE_MIGHT_BE_UNUSED
#elif defined( _CLANG )
#    define __B33_ATTRIBUTE_MIGHT_BE_UNUSED __attribute__( ( unused ) )
#else
#    error "Compiler isn't supported"
#endif

#if defined( _GNU )
#    define __B33_ATTRIBUTE_WARN_UNUSED __attribute__( ( warn_unused_result ) )
#elif defined( _MSVC )
#    define __B33_ATTRIBUTE_WARN_UNUSED _Check_return_
#elif defined( _CLANG )
#    define __B33_ATTRIBUTE_WARN_UNUSED __attribute__( ( warn_unused_result ) )
#else
#    error "Compiler isn't supported"
#endif

#endif
