#ifndef B33_GROUP_HPP
#define B33_GROUP_HPP

#include "B33Core.h"
#include "B33System.hpp"

namespace B33::System
{

// --------------------------------------------------------------------------------------------------------------------
template <typename SHARED_DATA, size_t POOL_SIZE = 64, typename... PER_OBJECT>
class GroupInstance
{
    template <typename T, typename... Ts>
    static constexpr size_t CountOccurrences = ( 0 + ... + ::std::is_same_v<T, Ts> );

    template <typename T, typename Tuple>
    struct TupleIndex;

    template <typename T, typename... Ts>
    struct TupleIndex<T, ::std::tuple<T, Ts...>>
    {
        static constexpr size_t value = 0;
    };

    template <typename T, typename U, typename... Ts>
    struct TupleIndex<T, ::std::tuple<U, Ts...>>
    {
        static constexpr size_t value = 1 + TupleIndex<T, ::std::tuple<Ts...>>::value;
    };

    struct Metadata
    {
        size_t uByteSize;
    };

    using AllocatedMemoryPerDataType = ::std::vector<char>;
    using DataVector                 = ::std::vector<AllocatedMemoryPerDataType>;
    using DataTable                  = ::std::vector<Metadata>;

  public:
    GroupInstance()
      : m_SharedData()
      , m_Data()
      , m_Table()
      , m_uItemsCount( 0 )
      , m_uReserved( 0 )
    {
        B33_TRACE( L"Instance created" );

        auto alloc = [ this ]<typename T>()
        {
            B33_ASSERT( ::std::is_trivially_copyable_v<T> );
            Metadata m = { 0 };

            m.uByteSize = sizeof( T );

            m_Table.push_back( m );
            m_Data.push_back( {} );
        };

        ( alloc.template operator()<PER_OBJECT>(), ... );
        ReallocateAllDataPools();
    }

    ~GroupInstance()
    {
        B33_TRACE( L"Instance destroyed" );
    }

  public:
    GroupInstance( GroupInstance && )                 = default;
    GroupInstance &operator=( GroupInstance && )      = default;
    GroupInstance( const GroupInstance & )            = default;
    GroupInstance &operator=( const GroupInstance & ) = default;

  public:
    const SHARED_DATA &GetSharedData() const
    {
        return m_SharedData;
    }

    SHARED_DATA &GetSharedData()
    {
        return m_SharedData;
    }

    size_t GetEntityCount() const
    {
        return m_uItemsCount;
    }

  public:
    template <typename T>
    T &GetValue( size_t entityId )
    {
        B33_ASSERT_MSG( ( CountOccurrences<T, PER_OBJECT...> == 1 ),
                        "Group: T appears zero or multiple times in PER_OBJECT — "
                        "use GetValue<Index>() or wrap duplicate primitive types in distinct tag structs." );

        constexpr size_t uIndex = TupleIndex<T, ::std::tuple<PER_OBJECT...>>::value;
        B33_ASSERT( entityId >= 0 && entityId < m_uItemsCount );

        char *base = m_Data[ uIndex ].data();

        return *reinterpret_cast<T *>( base + entityId * sizeof( T ) );
    }

    template <size_t Index>
    auto &GetValue( size_t entityId )
    {
        using T = ::std::tuple_element_t<Index, ::std::tuple<PER_OBJECT...>>;

        B33_ASSERT_MSG( ( Index < sizeof...( PER_OBJECT ) ), "Group: value index out of range" );

        B33_ASSERT( entityId >= 0 && entityId < m_uItemsCount );

        char *base = m_Data[ Index ].data();

        return *reinterpret_cast<T *>( base + entityId * sizeof( T ) );
    }

    size_t CreateNewEntitiy()
    {
        if ( m_uItemsCount >= m_uReserved )
        {
            ReallocateAllDataPools();
        }
        auto result = m_uItemsCount;
        ++m_uItemsCount;

        return result;
    }

  private:
    /**
     * @brief Reallocates pool of memory for the data type, we can do it as vectors are guaranteed to be contiguous
     *
     * @param mem Vector of data type
     * @param typeSize Size of type in bytes 
     */
    void ReallocatePool( AllocatedMemoryPerDataType &mem, size_t typeSize )
    {
        mem.resize( mem.size() + POOL_SIZE * typeSize );
    }

    void ReallocateAllDataPools()
    {
        for ( size_t i = 0; i < m_Data.size(); ++i )
        {
            ReallocatePool( m_Data[ i ], m_Table[ i ].uByteSize );
        }
        m_uReserved += POOL_SIZE;
    }

  private:
    SHARED_DATA m_SharedData  = {};
    DataVector  m_Data        = {};
    DataTable   m_Table       = {};
    size_t      m_uItemsCount = -1;
    size_t      m_uReserved   = -1;
};

// --------------------------------------------------------------------------------------------------------------------
template <typename SHARED_DATA, size_t POOL_SIZE = 64, typename... PER_OBJECT>
class Group
{
    using GroupInstanceT    = GroupInstance<SHARED_DATA, POOL_SIZE, PER_OBJECT...>;
    using InstanceSharedPtr = ::std::shared_ptr<GroupInstanceT>;

  public:
    Group()
    {
        if ( m_pInstance == nullptr )
        {
            m_pInstance = ::std::make_shared<GroupInstanceT>();
        }
        m_pInstanceLocal = m_pInstance;
        m_uIndex         = m_pInstance->CreateNewEntitiy();
    }

    ~Group()
    {
        const size_t uCountWithoutLocal = m_pInstanceLocal.use_count() - 1;
        if ( uCountWithoutLocal == 1 )
        {
            m_pInstance = nullptr;
        }
    }

  public:
    Group( Group && )                 = default;
    Group &operator=( Group && )      = default;
    Group( const Group & )            = default;
    Group &operator=( const Group & ) = default;

  public:
    size_t GetSelfIndex() const
    {
        return m_uIndex;
    }

    GroupInstanceT &GetMemory()
    {
        return *m_pInstanceLocal.get();
    }

  private:
    inline static InstanceSharedPtr m_pInstance      = nullptr;
    InstanceSharedPtr               m_pInstanceLocal = nullptr;
    size_t                          m_uIndex         = -1;
};


} // namespace B33::System

#endif // !B33_GROUP_HPP
