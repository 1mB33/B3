#if !defined( B33_EVENT_DISPATCHER_TESTS_HPP )
#    define B33_EVENT_DISPATCHER_TESTS_HPP

#    include "Tests/TestInstance.hpp"
#    include "Events/EventDispatcher.hpp"
#    include "Events/Event.hpp"
#    include "Debug/Assert.hpp"

TEST( EventDispatcher )
{
    static int Count = 0;

    class ClickerCounter
    {
      public:
        void Click()
        {
            ++Count;
        }
    };

    class SomeKindOfEvent : public B33::System::Event<SomeKindOfEvent>
    {
    };

    class AnotherEvent : public B33::System::Event<AnotherEvent>
    {
    };

    B33::System::EventDispatcher dispatcher = {};

    ClickerCounter counter = {};
    {
        auto handle1 = dispatcher.Register<SomeKindOfEvent>( +[]()
                                                             {
                                                                 ++Count;
                                                             } );

        auto handle2 = dispatcher.Register<SomeKindOfEvent>( +[]()
                                                             {
                                                                 ++Count;
                                                             } );

        auto handle3 = dispatcher.Register<AnotherEvent>( +[]()
                                                          {
                                                              ++Count;
                                                          } );

        auto handle4 = dispatcher.Register<SomeKindOfEvent>( &counter, &ClickerCounter::Click );
    }
    auto handle1 = dispatcher.Register<SomeKindOfEvent>( +[]()
                                                         {
                                                             ++Count;
                                                         } );

    auto handle2 = dispatcher.Register<SomeKindOfEvent>( +[]()
                                                         {
                                                             ++Count;
                                                         } );

    auto handle3 = dispatcher.Register<AnotherEvent>( +[]()
                                                      {
                                                          ++Count;
                                                      } );


    auto handle4 = dispatcher.Register<AnotherEvent>( &counter, &ClickerCounter::Click );

    for ( int i = 0; i < 10; ++i )
    {
        dispatcher.Trigger<SomeKindOfEvent>();
        dispatcher.Trigger<AnotherEvent>();
    }

    B33_ASSERT( Count == 40 );
}

#endif // !defined ( B33_EVENT_DISPATCHER_TESTS_HPP )
