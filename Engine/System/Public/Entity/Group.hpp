#ifndef B33_GROUP_HPP
#define B33_GROUP_HPP

#include "B33Core.h"
#include "B33System.hpp"
#include "Debug/Assert.hpp"

namespace B33::System
{

template <typename SHARED_DATA, size_t POOL_SIZE = 64, typename... PER_OBJECT>
class Group
{
    struct Metadata
    {
        size_t uByteSize;
    };

    using AllocatedMemoryPerDataType = ::std::vector<char>;
    using DataVector                 = ::std::vector<AllocatedMemoryPerDataType>;
    using DataTable                  = ::std::vector<Metadata>;

  public:
    Group()
      : m_SharedData()
      , m_Data()
      , m_Table()
    {
        auto alloc = [ this ]<typename T>()
        {
            Metadata m = { 0 };

            m.uByteSize = sizeof( T );

            m_Table.push_back( m );
            m_Data.push_back( {} );
            ReallocatePool( m_Data.back(), m.uByteSize );
        };

        ( alloc.template operator()<PER_OBJECT>(), ... );
    }

    ~Group() = default;

  public:
    Group( Group && )                 = default;
    Group &operator=( Group && )      = default;
    Group( const Group & )            = default;
    Group &operator=( const Group & ) = default;

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
        return m_Data.size();
    }

  public:
    // const DATA &GetEntityData( size_t i ) const
    // {
    //     B33_ASSERT( i < m_Data.size() );
    //     return m_Data[ i ];
    // }

    // DATA &GetEntityData( size_t i )
    // {
    //     B33_ASSERT( i < m_Data.size() );
    //     return m_Data[ i ];
    // }

    size_t CreateNewEntitiy()
    {
        size_t result = m_Data.size();
        //m_Data.push_back( DATA() );
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

  private:
    SHARED_DATA m_SharedData = {};
    DataVector  m_Data       = {};
    DataTable   m_Table      = {};
};

} // namespace B33::System

#endif // !B33_GROUP_HPP
