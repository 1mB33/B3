#ifndef B33_GROUP_HPP
#define B33_GROUP_HPP

#include "B33System.hpp"
#include "Debug/Assert.hpp"

namespace B33::System
{

template <typename DATA, typename SHARED_DATA>
class Group
{
    using GroupData = ::std::vector<DATA>;

  public:
    Group()
      : m_Data()
      , m_SharedData()
    {
    }

    ~Group() = default;

  public:
    Group( Group && )                 = default;
    Group &operator=( Group && )      = default;
    Group( const Group & )            = default;
    Group &operator=( const Group & ) = default;

  public:
    const DATA &GetEntityData( size_t i ) const
    {
        B33_ASSERT( i < m_Data.size() );
        return m_Data[ i ];
    }

    DATA &GetEntityData( size_t i )
    {
        B33_ASSERT( i < m_Data.size() );
        return m_Data[ i ];
    }

    const SHARED_DATA &GetSharedData() const
    {
        return m_SharedData;
    }

    SHARED_DATA &GetSharedData()
    {
        return m_SharedData;
    }

  public:
    size_t CreateNewEntitiy()
    {
        size_t result = m_Data.size();
        m_Data.push_back( DATA() );
        return result;
    }

  private:
    GroupData   m_Data       = {};
    SHARED_DATA m_SharedData = {};
};

} // namespace B33::System

#endif // !B33_GROUP_HPP
