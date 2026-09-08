#if !defined( B33_EVENT_DISPATCHER_TESTS_HPP )
#    define B33_EVENT_DISPATCHER_TESTS_HPP

#    include <Tests/TestInstance.hpp>
#    include "EmptyCanvas.hpp"
#    include "Window/WindowPolicy/BasicSystemPolicy.hpp"
#    include "Window/WindowPolicy/BorderlessGameSystemPolicy.hpp"

TEST( Windows_InstanceCreation )
{
    B33::App::EmptyCanvas<true, ::B33::App::DefaultGameSystemWindowPolicy> window( L"Test", 1, 1 );
    window.Create();
    for ( int32_t i = 0; i < 40; ++i )
    {
        window.Update( 0.f );
    }

    B33_ASSERT( window.GetWindowDesc()->Data.bIsAlive == true );
}

TEST( Windows_BehaviorChange )
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

TEST( Windows_RecreateWindowBad )
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

TEST( Windows_RecreateWindowGood )
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
