#if !defined( B33_JOB_SYSTEM_HPP )
#    define B33_JOB_SYSTEM_HPP

#    include "B33Core.h"

namespace B33::Core
{

class JobSystem
{
    using ABool        = ::std::atomic_bool;
    using Thread       = ::std::thread;
    using Mutex        = ::std::mutex;
    using ConditionVar = ::std::condition_variable;
    template <typename T>
    using Vector = ::std::vector<T>;

    class IRunnableJob
    {
      public:
        virtual ~IRunnableJob() noexcept = default;

      public:
        virtual void Call() = 0;
    };

    template <typename... ARGS>
    class RunnableJob final : public IRunnableJob
    {
        template <typename... T>
        using Tuple = ::std::tuple<T...>;

        template <typename T>
        constexpr decltype( auto ) Forward( T &arg ) noexcept
        {
            return ::std::forward<T>( arg );
        }

        template <typename T>
        constexpr decltype( auto ) Forward( T &&arg ) noexcept
        {
            return ::std::forward<T>( arg );
        }

        constexpr decltype( auto ) MakeTuple() noexcept
        {
            return ::std::make_tuple();
        }

        template <typename... U>
        constexpr decltype( auto ) MakeTuple( U &&...arg ) noexcept
        {
            return ::std::make_tuple( Forward<U>( arg )... );
        }

      public:
        explicit constexpr RunnableJob( void ( *func )( ARGS... ), ARGS... argv )
          : m_FuncPtr( func )
          , m_Params( MakeTuple( argv... ) )
        {
        }

      public:
        virtual void Call() override final
        {
            using ::std::apply;

            apply( m_FuncPtr, m_Params );
        }

      private:
        void ( *m_FuncPtr )( ARGS... ) = nullptr;
        Tuple<ARGS...> m_Params        = {};
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
        Thread       ThreadHandle;
        Mutex        LocalMutex;
        ABool        IsWorking;
        ABool        IsFree;
        ConditionVar Condition;
        Job          CurrentJob;
    };

  private:
    __B33_API void PushJobInternal( Job newJob );

    static void JobProcessorLoop( Mutex        &mutex,
                                  ConditionVar &condition,
                                  ABool        &IsWorking,
                                  ABool        &IsFree,
#    if defined( _B33_DEBUG )
                                  ABool &IsError,
#    endif
                                  Job &currentJob );

  private:
    Vector<JobProcessor> m_Threads = {};
    usize                m_uHead   = -1;
    ABool                m_IsError = false;
};

} // namespace B33::Core

#endif // !B33_JOB_SYSTEM_HPP
