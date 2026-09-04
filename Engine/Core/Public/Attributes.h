#if !defined( B33_ATTRIBUTES_H )
#    define B33_ATTRIBUTES_H

#    include "MinimalSystemIncludes.h"

#    if defined( _GNU )
#        define __B33_ATTRIBUTE_MIGHT_BE_UNUSED __attribute__( ( unused ) )
#    elif defined( _MSVC )
#        define __B33_ATTRIBUTE_MIGHT_BE_UNUSED
#    elif defined( _CLANG )
#        define __B33_ATTRIBUTE_MIGHT_BE_UNUSED __attribute__( ( unused ) )
#    else
#        error "Compiler isn't supported"
#    endif

#    if defined( _GNU )
#        define __B33_ATTRIBUTE_WARN_UNUSED __attribute__( ( warn_unused_result ) )
#    elif defined( _MSVC )
#        define __B33_ATTRIBUTE_WARN_UNUSED _Check_return_
#    elif defined( _CLANG )
#        define __B33_ATTRIBUTE_WARN_UNUSED __attribute__( ( warn_unused_result ) )
#    else
#        error "Compiler isn't supported"
#    endif

#    if defined( _B33_DEBUG )
#        if defined( _GNU )
#            define __B33_BEBUG_BREAK_POINT( MSG )                                                                     \
                do                                                                                                     \
                {                                                                                                      \
                    perror( MSG );                                                                                     \
                    raise( SIGTRAP );                                                                                  \
                } while ( 0 );
#        elif defined( _MSVC )
#            define __B33_BEBUG_BREAK_POINT( MSG )                                                                     \
                do                                                                                                     \
                {                                                                                                      \
                    perror( MSG );                                                                                     \
                    __debugbreak;                                                                                      \
                } while ( 0 );
#        elif defined( _CLANG )
#            define __B33_BEBUG_BREAK_POINT( MSG )                                                                     \
                do                                                                                                     \
                {                                                                                                      \
                    perror( MSG );                                                                                     \
                    raise( SIGTRAP );                                                                                  \
                } while ( 0 );
#        else
#            error "Compiler isn't supported"
#        endif
#    else
#        if defined( _GNU )
#            define __B33_BEBUG_BREAK_POINT( MSG )                                                                     \
                do                                                                                                     \
                {                                                                                                      \
                    perror( MSG );                                                                                     \
                } while ( 0 );
#        elif defined( _MSVC )
#            define __B33_BEBUG_BREAK_POINT( MSG )                                                                     \
                do                                                                                                     \
                {                                                                                                      \
                    perror( MSG );                                                                                     \
                } while ( 0 );
#        elif defined( _CLANG )
#            define __B33_BEBUG_BREAK_POINT( MSG )                                                                     \
                do                                                                                                     \
                {                                                                                                      \
                    perror( MSG );                                                                                     \
                } while ( 0 );
#        else
#            error "Compiler isn't supported"
#        endif
#    endif

#endif
