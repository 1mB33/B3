#include "Debug/Assert.hpp"
#if !defined( B33_BORROWED_HPP )
#    define B33_BORROWED_HPP

namespace B33::Core
{
template <class GUARDED_T>
class Borrowed
{
    using Mutex = ::std::mutex;

  public:
    explicit Borrowed()
      : m_mMutex( nullptr )
      , m_Object( nullptr )
      , m_bValid( false )
    {
    }

    explicit Borrowed( Mutex *mutex, GUARDED_T *object )
      : m_mMutex( mutex )
      , m_Object( object )
      , m_bValid( true )
    {
        m_mMutex->lock();
    }

    ~Borrowed()
    {
        if ( m_bValid )
            m_mMutex->unlock();
    }

  public:
    Borrowed( Borrowed &&other )
      : m_mMutex( other.m_mMutex )
      , m_Object( other.m_Object )
      , m_bValid( other.m_bValid )
    {
        other.m_bValid = false;
    }

    Borrowed &operator=( Borrowed &&other )
    {
        if ( m_bValid )
            m_mMutex->unlock();

        m_mMutex = other.m_mMutex;
        m_Object = other.m_Object;
        m_bValid = other.m_bValid;

        other.m_bValid = false;
    }

    Borrowed( const Borrowed & )            = delete;
    Borrowed &operator=( const Borrowed & ) = delete;

  public:
    GUARDED_T &Get()
    {
        B33_ASSERT( m_bValid );
        return *m_Object;
    }

  public:
    GUARDED_T *operator->()
    {
        B33_ASSERT( m_bValid );
        return m_Object;
    }

  private:
    Mutex     *m_mMutex;
    GUARDED_T *m_Object;
    bool       m_bValid;
};
} // namespace B33::Core

#endif //! B33_BORROWED_HPP
