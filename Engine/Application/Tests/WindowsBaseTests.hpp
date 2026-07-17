#if !defined( B33_EVENT_DISPATCHER_TESTS_HPP )
#    define B33_EVENT_DISPATCHER_TESTS_HPP

#    include "B33Core.h"
#    include "Exception.hpp"
#    include "EmptyCanvas.hpp"
#    include "Window/WindowPolicy/BasicSystemPolicy.hpp"
#    include "Window/WindowPolicy/BorderlessGameSystemPolicy.hpp"
#    include "Tests/TestInstance.hpp"
#    include "Debug/Assert.hpp"

TEST( WindowsInstanceCreation )
{
    B33::App::EmptyCanvas<true, ::B33::App::DefaultGameSystemWindowPolicy> window( L"Test", 1, 1 );
    window.Create();
    for ( int32_t i = 0; i < 40; ++i )
    {
        window.Update( 0.f );
    }

    B33_ASSERT( window.GetWindowDesc()->Data.bIsAlive == true );
}

TEST( WindowsBehaviorChange )
{
    __B33_ATTRIBUTE_MIGHT_BE_UNUSED auto dgswp = ::B33::App::DefaultGameSystemWindowPolicy();

    B33::App::EmptyCanvas<true, ::B33::App::DefaultGameSystemWindowPolicy> window( L"Test", 1, 1 );
    window.Create();
    for ( int32_t i = 0; i < 40; ++i )
    {
        window.Update( 0.f );
    }
    window.ChangePolicy<B33::App::BorderlessGameSystemPolicy>();
    for ( int32_t i = 0; i < 40; ++i )
    {
        window.Update( 0.f );
    }

    B33_ASSERT( window.GetWindowDesc()->Data.bIsAlive == true );
    B33_ASSERT( typeid( window.GetPolicy() ) != typeid( dgswp ) );
}

TEST( WindowRecreateWindowBad )
{
    B33::App::EmptyCanvas<true, ::B33::App::DefaultGameSystemWindowPolicy> window( L"Test", 1, 1 );
    window.Create();
    for ( int32_t i = 0; i < 40; ++i )
    {
        window.Update( 0.f );
    }
    window.Create();
    for ( int32_t i = 0; i < 40; ++i )
    {
        window.Update( 0.f );
    }

    B33_ASSERT( window.GetWindowDesc()->Data.bIsAlive == true );
}

TEST( WindowRecreateWindowGood )
{
    B33::App::EmptyCanvas<true, ::B33::App::DefaultGameSystemWindowPolicy> window( L"Test", 1, 1 );
    window.Create();
    for ( int32_t i = 0; i < 40; ++i )
    {
        window.Update( 0.f );
    }

    window.Destroy();
    for ( int32_t i = 0; i < 40; ++i )
    {
        window.Update( 0.f );
    }
    B33_ASSERT( window.GetWindowDesc()->Data.bIsAlive != true );

    window.Create();
    window.Update( 0.f );

    B33_ASSERT( window.GetWindowDesc()->Data.bIsAlive == true );
}

#endif // !defined ( B33_EVENT_DISPATCHER_TESTS_HPP )
