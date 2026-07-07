#if !defined( B33_EVENT_DISPATCHER_TESTS_HPP )
#    define B33_EVENT_DISPATCHER_TESTS_HPP

#    include "B33Core.h"
#    include "Exception.hpp"
#    include "EmptyCanvas.hpp"
#    include "Window/WindowPolicy/BasicSystemPolicy.hpp"
#    include "Tests/TestInstance.hpp"
#    include "Debug/Assert.hpp"

TEST( WindowsInstanceCreation )
{
    B33::App::EmptyCanvas<true, ::B33::App::DefaultGameSystemWindowPolicy> window( L"Test", 1, 1 );
    window.Create();
    window.Update( 0.f );

    B33_ASSERT( window.GetWindowDesc()->bIsAlive == true );
}

#endif // !defined ( B33_EVENT_DISPATCHER_TESTS_HPP )
