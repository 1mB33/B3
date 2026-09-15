#include "B33System.hpp"
#include "JobSystem.hpp"

namespace B33::System
{

using namespace std;
using namespace B33;

void JobSystem::JobProcessorLoop( Mutex        &mutex,
                                  ConditionVar &condition,
                                  bool         &IsWorking,
                                  bool         &IsFree,
#if defined( _B33_DEBUG )
                                  ABool &IsError,
#endif
                                  Job &currentJob )
{
    Job executedJob;

#if defined( _B33_DEBUG )
    try
    {
#endif
        while ( 1 )
        {
            {
                unique_lock ul( mutex );
                condition.wait( ul,
                                [ & ]
                                {
                                    return !IsFree || !IsWorking;
                                } );
                if ( !IsWorking )
                {
                    return;
                }
                executedJob.Runnable = currentJob.Runnable;
                currentJob.Runnable  = nullptr;
            }
            if ( executedJob.Runnable != nullptr )
            {
                executedJob.Runnable->Call();
                delete executedJob.Runnable;
                executedJob.Runnable = nullptr;
            }
            {
                unique_lock ul( mutex );
                IsFree = true;
            }
            condition.notify_one();
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
    IsError.store( true, memory_order_relaxed );
    IsFree    = true;
    IsWorking = false;
    condition.notify_all();
    B33_ERROR( L"Exiting on job processor, thread id %d", this_thread::get_id() );
#endif
}

JobSystem::JobSystem()
  : m_Threads( max( Thread::hardware_concurrency(), 1u ) )
  , m_uHead( 0 )
  , m_IsError( false )
{
    B33_TRACE( L"JobSystem::JobSystem(): Job processors count %d", m_Threads.size() );
    for ( auto &t : m_Threads )
    {
        B33_TRACE( L"JobSystem::JobSystem(): Starting one of job processors" );
        t.IsFree       = true;
        t.IsWorking    = true;
        t.CurrentJob   = { nullptr };
        t.ThreadHandle = Thread( &JobSystem::JobProcessorLoop,
                                 ref( t.LocalMutex ),
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
        B33_TRACE( L"JobSystem::~JobSystem(): Stopping one of job processors: %d", t.ThreadHandle.get_id() );
        if ( m_IsError.load( memory_order_acquire ) )
        {
            t.ThreadHandle.~thread();
        }
        else if ( t.ThreadHandle.joinable() )
        {
            {
                unique_lock ul( t.LocalMutex );
                t.IsWorking = false;
            }
            t.Condition.notify_all();
            t.ThreadHandle.join();
        }
    }
    B33_TRACE( L"JobSystem::~JobSystem(): Finished" );
}

void JobSystem::BlockAndWait()
{
    for ( auto &t : m_Threads )
    {
        unique_lock ul( t.LocalMutex );
        t.Condition.wait( ul,
                          [ & ]()
                          {
                              return t.IsFree;
                          } );
    }
#if defined( _B33_DEBUG )
    if ( m_IsError.load( memory_order_acquire ) )
    {
        B33_ERROR( L"On job processor, error detected" );
        throw B33_EXCEPT( "Job system internal error" );
    }
#endif
}

void JobSystem::PushJobInternal( Job newJob )
{
#if defined( _B33_DEBUG )
    if ( m_IsError.load( memory_order_acquire ) )
    {
        B33_ERROR( L"On job processor, error detected" );
        throw B33_EXCEPT( "Job system internal error" );
    }
#endif

    auto &headThread = m_Threads[ m_uHead ];

    {
        unique_lock ul( headThread.LocalMutex );
        headThread.Condition.wait( ul,
                                   [ & ]()
                                   {
                                       return headThread.IsFree;
                                   } );
        B33_TRACE( L"Pushing new job to the processors %p", headThread.CurrentJob.Runnable );
        headThread.CurrentJob = std::move( newJob );
        headThread.IsFree     = false;
        headThread.Condition.notify_one();
    }

    m_uHead = ( m_uHead + 1 ) % m_Threads.size();
}

}; // namespace B33::System
