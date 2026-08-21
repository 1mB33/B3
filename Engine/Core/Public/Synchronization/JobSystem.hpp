#include <memory>
#if !defined( B33_JOB_SYSTEM_HPP )
#    define B33_JOB_SYSTEM_HPP

#    include "B33Core.h"

namespace B33::Core
{

class JobSystem
{
    class IRunnableJob
    {
      public:
        virtual ~IRunnableJob() = default;

      public:
        virtual void Call() = 0;
    };

    template <typename... ARGS>
    class RunnableJob final : public IRunnableJob
    {
      public:
        explicit constexpr RunnableJob( void ( *func )( ARGS... ), ARGS... argv )
          : m_FuncPtr { func }
          , m_Params { std::make_tuple( argv... ) }
        {
        }

      public:
        virtual void Call() override final
        {
            std::apply( m_FuncPtr, m_Params );
        }

      private:
        void ( *m_FuncPtr )( ARGS... ) = nullptr;
        std::tuple<ARGS...> m_Params   = {};
    };

  public:
    __B33_API JobSystem();

    __B33_API ~JobSystem();

  public:
    template <typename... ARGS>
    void PushJob( void ( *func )( ARGS... ), ARGS... argv )
    {
        B33_TRACE( L"JobSystem::PushJob() Pushing new job" );
        Job newJob      = {};
        newJob.Runnable = new RunnableJob( func, argv... );

        PushJobInternal( ::std::move( newJob ) );
    }

    __B33_API void BlockAndWait();

  private:
    struct Job
    {
        IRunnableJob *Runnable;
    };

    struct JobProcessor
    {
        ::std::thread             Thread;
        ::std::mutex              Mutex;
        ::std::atomic_bool        IsWorking;
        ::std::atomic_bool        IsFree;
        ::std::condition_variable Condition;
        Job                       CurrentJob;
    };

  private:
    __B33_API void PushJobInternal( Job newJob );

    static void JobProcessorLoop( ::std::mutex              &mutex,
                                  ::std::condition_variable &condition,
                                  ::std::atomic_bool        &IsWorking,
                                  ::std::atomic_bool        &IsFree,
#    if defined( _B33_DEBUG )
                                  ::std::atomic_bool &IsError,
#    endif
                                  Job &currentJob );

  private:
    ::std::vector<JobProcessor> m_Threads = {};
    ::size_t                    m_uHead   = -1;
    ::std::atomic_bool          m_IsError = false;
};

} // namespace B33::Core

#endif // !B33_JOB_SYSTEM_HPP
