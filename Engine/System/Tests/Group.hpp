#if !defined( B33_GROUP_TESTS_HPP )
#    define B33_GROUP_TESTS_HPP

#    include <Tests/TestInstance.hpp>
#    include "Entity/Group.hpp"

namespace B33::Tests::GroupTests
{


struct Position
{
    float x;
    float y;
};

struct Velocity
{
    float dx;
    float dy;
};

struct SharedTag_MultiGroup
{
};

struct SharedTag_SharedData
{
    int uFrameCount = 0;
};

struct SharedTag_LifecycleA
{
};

struct SharedTag_LifecycleB
{
};

struct SharedTag_CopyMove
{
};

} // namespace B33::Tests::GroupTests

TEST( GroupMemory_DefaultConstructionStartsEmpty )
{
    using namespace ::B33::Tests::GroupTests;
    ::B33::System::GroupMemory<int, 64, Position> memory;

    B33_ASSERT( memory.GetEntityCount() == 0 );
}

TEST( GroupMemory_CreateNewEntityReturnsSequentialIndices )
{
    using namespace ::B33::Tests::GroupTests;
    ::B33::System::GroupMemory<int, 64, Position> memory;

    const size_t uFirst  = memory.CreateNewEntitiy();
    const size_t uSecond = memory.CreateNewEntitiy();
    const size_t uThird  = memory.CreateNewEntitiy();

    B33_ASSERT( uFirst == 0 );
    B33_ASSERT( uSecond == 1 );
    B33_ASSERT( uThird == 2 );
    B33_ASSERT( memory.GetEntityCount() == 3 );
}

TEST( GroupMemory_GetValueByTypeReadsBackWrittenData )
{
    using namespace ::B33::Tests::GroupTests;
    ::B33::System::GroupMemory<int, 64, Position, Velocity> memory;

    const size_t uEntity = memory.CreateNewEntitiy();

    memory.GetValue<Position>( uEntity ) = { 1.0f, 2.0f };
    memory.GetValue<Velocity>( uEntity ) = { 0.5f, -0.5f };

    B33_ASSERT( memory.GetValue<Position>( uEntity ).x == 1.0f );
    B33_ASSERT( memory.GetValue<Position>( uEntity ).y == 2.0f );
    B33_ASSERT( memory.GetValue<Velocity>( uEntity ).dx == 0.5f );
    B33_ASSERT( memory.GetValue<Velocity>( uEntity ).dy == -0.5f );
}

TEST( GroupMemory_GetValueByIndexMatchesGetValueByType )
{
    using namespace ::B33::Tests::GroupTests;
    ::B33::System::GroupMemory<int, 64, Position, Velocity> memory;

    const size_t uEntity = memory.CreateNewEntitiy();

    memory.GetValue<0>( uEntity ) = { 3.0f, 4.0f };
    memory.GetValue<1>( uEntity ) = { -1.0f, 2.0f };

    B33_ASSERT( memory.GetValue<Position>( uEntity ).x == 3.0f );
    B33_ASSERT( memory.GetValue<Position>( uEntity ).y == 4.0f );
    B33_ASSERT( memory.GetValue<Velocity>( uEntity ).dx == -1.0f );
    B33_ASSERT( memory.GetValue<Velocity>( uEntity ).dy == 2.0f );
}

TEST( GroupMemory_DifferentEntitiesDoNotAliasEachOther )
{
    using namespace ::B33::Tests::GroupTests;
    ::B33::System::GroupMemory<int, 64, Position> memory;

    const size_t uA = memory.CreateNewEntitiy();
    const size_t uB = memory.CreateNewEntitiy();

    memory.GetValue<Position>( uA ) = { 10.0f, 20.0f };
    memory.GetValue<Position>( uB ) = { 30.0f, 40.0f };

    B33_ASSERT( memory.GetValue<Position>( uA ).x == 10.0f );
    B33_ASSERT( memory.GetValue<Position>( uB ).x == 30.0f );
}

TEST( GroupMemory_GetDataReturnsContiguousBaseForAllEntities )
{
    using namespace ::B33::Tests::GroupTests;
    ::B33::System::GroupMemory<int, 64, Position> memory;

    const size_t uA = memory.CreateNewEntitiy();
    const size_t uB = memory.CreateNewEntitiy();

    memory.GetValue<Position>( uA ) = { 1.0f, 1.0f };
    memory.GetValue<Position>( uB ) = { 2.0f, 2.0f };

    Position *pData = memory.GetData<Position>();

    B33_ASSERT( pData[ uA ].x == 1.0f );
    B33_ASSERT( pData[ uB ].x == 2.0f );
}

TEST( GroupMemory_SharedDataDefaultsAndMutates )
{
    ::B33::System::GroupMemory<int, 64, ::B33::Tests::GroupTests::Position> memory;

    B33_ASSERT( memory.GetSharedData() == 0 );

    memory.GetSharedData() = 42;
    B33_ASSERT( memory.GetSharedData() == 42 );

    const auto &constMemory = memory;
    B33_ASSERT( constMemory.GetSharedData() == 42 );
}

TEST( GroupMemory_SurvivesReallocationAcrossPoolBoundary )
{
    using namespace ::B33::Tests::GroupTests;


    ::B33::System::GroupMemory<int, 4, Position> memory;
    static constexpr size_t                      uCount = 4 * 5 + 3;

    for ( size_t i = 0; i < uCount; ++i )
    {
        const size_t uEntity                 = memory.CreateNewEntitiy();
        memory.GetValue<Position>( uEntity ) = { static_cast<float>( uEntity ), 0.0f };
    }

    B33_ASSERT( memory.GetEntityCount() == uCount );


    for ( size_t i = 0; i < uCount; ++i )
    {
        B33_ASSERT( memory.GetValue<Position>( i ).x == static_cast<float>( i ) );
    }
}

TEST( Group_MultipleInstancesShareSameUnderlyingMemory )
{
    using GroupT =
        ::B33::System::Group<::B33::Tests::GroupTests::SharedTag_MultiGroup, 64, ::B33::Tests::GroupTests::Position>;

    GroupT first;
    GroupT second;
    GroupT third;

    B33_ASSERT( first.GetSelfIndex() == 0 );
    B33_ASSERT( second.GetSelfIndex() == 1 );
    B33_ASSERT( third.GetSelfIndex() == 2 );

    first.GetMemory().GetValue<::B33::Tests::GroupTests::Position>( first.GetSelfIndex() ) = { 7.0f, 8.0f };


    B33_ASSERT( &first.GetMemory() == &second.GetMemory() );
    B33_ASSERT( &first.GetMemory() == &third.GetMemory() );
    B33_ASSERT( second.GetMemory().GetValue<::B33::Tests::GroupTests::Position>( first.GetSelfIndex() ).x == 7.0f );
}

TEST( Group_SharedDataVisibleAcrossInstances )
{
    using GroupT =
        ::B33::System::Group<::B33::Tests::GroupTests::SharedTag_SharedData, 64, ::B33::Tests::GroupTests::Position>;

    GroupT first;
    first.GetMemory().GetSharedData().uFrameCount = 5;

    GroupT second;
    B33_ASSERT( second.GetMemory().GetSharedData().uFrameCount == 5 );
}

TEST( Group_MemoryResetsAfterAllInstancesDestroyed )
{
    using GroupT =
        ::B33::System::Group<::B33::Tests::GroupTests::SharedTag_LifecycleA, 64, ::B33::Tests::GroupTests::Position>;

    {
        GroupT first;
        GroupT second;
        B33_ASSERT( first.GetSelfIndex() == 0 );
        B33_ASSERT( second.GetSelfIndex() == 1 );
    }

    GroupT third;
    B33_ASSERT( third.GetSelfIndex() == 0 );
}

TEST( Group_CopyConstructionSharesInstanceAndIndex )
{
    using GroupT =
        ::B33::System::Group<::B33::Tests::GroupTests::SharedTag_CopyMove, 64, ::B33::Tests::GroupTests::Position>;

    GroupT original;
    GroupT copy = original;

    B33_ASSERT( copy.GetSelfIndex() == original.GetSelfIndex() );
    B33_ASSERT( &copy.GetMemory() == &original.GetMemory() );

    copy.GetMemory().GetValue<::B33::Tests::GroupTests::Position>( copy.GetSelfIndex() ) = { 9.0f, 9.0f };
    B33_ASSERT( original.GetMemory().GetValue<::B33::Tests::GroupTests::Position>( original.GetSelfIndex() ).x ==
                9.0f );
}

TEST( Group_MoveConstructionTransfersOwnership )
{
    using GroupT =
        ::B33::System::Group<::B33::Tests::GroupTests::SharedTag_LifecycleB, 64, ::B33::Tests::GroupTests::Position>;

    GroupT       source;
    const size_t uIndex = source.GetSelfIndex();

    GroupT moved = ::std::move( source );

    B33_ASSERT( moved.GetSelfIndex() == uIndex );
    moved.GetMemory().GetValue<::B33::Tests::GroupTests::Position>( uIndex ) = { 12.0f, 0.0f };
    B33_ASSERT( moved.GetMemory().GetValue<::B33::Tests::GroupTests::Position>( uIndex ).x == 12.0f );
}

#endif
