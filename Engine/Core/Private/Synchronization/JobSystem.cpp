#include "B33Core.h"
#include "Debug/Logger.hpp"
#include "Synchronization/JobSystem.hpp"
#include <thread>

namespace B33::Core
{

using namespace std;
using namespace B33;

void JobSystem::JobProcessorLoop( mutex              &mutex,
                                  condition_variable &condition,
                                  atomic_bool        &IsWorking,
                                  atomic_bool        &IsFree,
#if defined( _B33_DEBUG )
                                  atomic_bool &IsError,
#endif
                                  Job &currentJob )
{
#if defined( _B33_DEBUG )
    try
    {
#endif
        IsFree.store( true );
        while ( 1 )
        {
            unique_lock ul( mutex );
            condition.wait( ul,
                            [ & ]
                            {
                                return !IsFree.load() || !IsWorking.load();
                            } );

            if ( currentJob.Runnable != nullptr )
            {
                currentJob.Runnable->Call();
                delete currentJob.Runnable;
                currentJob.Runnable = nullptr;
            }
            if ( !IsWorking.load() )
            {
                return;
            }
            IsFree.store( true );
            condition.notify_all();
        }
#if defined( _B33_DEBUG )
    }
    catch ( ::std::exception &e )
    {
        B33_ERROR( L"On job processor, job %p failed, thread id %d", currentJob.Runnable, this_thread::get_id() );
        B33_ERROR( L"On what?: %s", e.what() );
    }
    catch ( ... )
    {
        B33_ERROR( L"On job processor, job %p failed, thread id %d", currentJob.Runnable, this_thread::get_id() );
    }
    IsError.store( true );
    IsFree.store( true );
    IsWorking.store( false );
    condition.notify_all();
    B33_ERROR( L"Exiting on job processor, thread id %d", this_thread::get_id() );
#endif
}

JobSystem::JobSystem()
  : m_Threads( ::std::thread::hardware_concurrency() - 2 )
  , m_uHead( 0 )
  , m_IsError( false )
{
    for ( auto &t : m_Threads )
    {
        B33_TRACE( L"JobSystem::JobSystem(): Starting one of job processors" );
        t.IsFree.store( true );
        t.IsWorking.store( true );
        t.CurrentJob = { nullptr };
        t.Thread     = thread( &JobSystem::JobProcessorLoop,
                               ref( t.Mutex ),
                               ref( t.Condition ),
                               ref( t.IsWorking ),
                               ref( t.IsFree ),
#if defined( _B33_DEBUG )
                           ref( m_IsError ),
#endif
                           ref( t.CurrentJob ) );
    }
}

JobSystem::~JobSystem()
{
    for ( auto &t : m_Threads )
    {
        B33_TRACE( L"JobSystem::~JobSystem(): Stopping one of job processors: %d", t.Thread.get_id() );
        if ( m_IsError.load() )
        {
            t.Thread.~thread();
        }
        else if ( t.Thread.joinable() )
        {
            t.IsWorking.store( false );
            t.Condition.notify_all();
            t.Thread.join();
        }
    }
    B33_TRACE( L"JobSystem::~JobSystem(): Finished" );
}

void JobSystem::BlockAndWait()
{
    for ( auto &t : m_Threads )
    {
        if ( !t.IsFree.load() )
        {
            unique_lock ul( t.Mutex );
            t.Condition.wait( ul,
                              [ & ]()
                              {
                                  return t.IsFree.load();
                              } );
        }
    }
#if defined( _B33_DEBUG )
    if ( m_IsError.load() )
    {
        B33_ERROR( L"On job processor, error detected" );
        throw B33_EXCEPT( "Job system internal error" );
    }
#endif
}

void JobSystem::PushJobInternal( Job newJob )
{
#if defined( _B33_DEBUG )
    if ( m_IsError.load() )
    {
        B33_ERROR( L"On job processor, error detected" );
        throw B33_EXCEPT( "Job system internal error" );
    }
#endif

    auto &headThread = m_Threads[ m_uHead ];

    if ( !headThread.IsFree.load() )
    {
        unique_lock ul( headThread.Mutex );
        headThread.Condition.wait( ul,
                                   [ & ]()
                                   {
                                       return headThread.IsFree.load();
                                   } );
    }

    headThread.CurrentJob = std::move( newJob );
    B33_TRACE( L"Pushing new job to the processors %p", headThread.CurrentJob.Runnable );
    headThread.IsFree.store( false );
    headThread.Condition.notify_all();

    m_uHead = ( m_uHead + 1 ) % m_Threads.size();
}

}; // namespace B33::Core
