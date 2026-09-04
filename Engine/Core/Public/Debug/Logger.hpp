#if !defined( B33_LOGGER_H )
#    define B33_LOGGER_H

#    include "B33CoreMinimal.h"

namespace B33::Core::Debug
{

enum ESeverity
{
    Info    = 1,
    Trace   = Info << 1,
    Warning = Trace << 1,
    Error   = Warning << 1,
};

class Logger
{
    using Clock          = ::std::chrono::system_clock;
    using ClockTimePoint = ::std::chrono::time_point<Clock>;
    using String         = ::std::string;
    using WString        = ::std::wstring;
    using ABool          = ::std::atomic_bool;
    using Thread         = ::std::thread;
    using Mutex          = ::std::mutex;
    using ConditionVar   = ::std::condition_variable;
    template <typename T>
    using Queue = ::std::queue<T>;
    template <typename T, typename U>
    using UMap = ::std::unordered_map<T, U>;

    struct LogStruct
    {
        ClockTimePoint TimeStamp;
        ESeverity      Sev;
        const char    *pszFile;
        const wchar_t *pwszFmt;
        wchar_t       *pwszMessage;
    };

    constexpr static inline u32 MaxTraceMessages = 15;

    __B33_API Logger();

    Logger( const Logger & ) = delete;
    Logger( Logger && )      = delete;

    Logger &operator=( const Logger & ) = delete;
    Logger &operator=( Logger && )      = delete;

  public:
    __B33_API static Logger &Get() noexcept;

    __B33_API ~Logger();

  public:
    /**
     * @brief Logs message to the file.
     *
     * @param sev Severity tag of the log message
     * @param pwszFmt Format for the message or message itself, written with `vswprintf` standard
     * @param ... Variables to be used in the format
     */
    __B33_API void Log( const ESeverity sev, const wchar_t wszFmt[], ... ) noexcept;

    /**
     * @brief Logs message to the file.
     *
     * @param file Name of file from which the message come
     * @param sev Severity tag of the log message
     * @param pwszFmt Format for the message or message itself, written with `vswprintf` standard
     * @param ... Variables to be used in the format
     */
    __B33_API void Log( const char szFile[], const ESeverity sev, const wchar_t wszFmt[], ... ) noexcept;

    __B33_API void Flush();

  private:
    String CreateDatePreFix() const;

    const wchar_t *GetTag( const ESeverity sev ) const;

    const char *GetFileName( const char *szFile ) const;

    const WString Stringify( const LogStruct &ls ) const;

    const WString StringifyAndColorize( const LogStruct &ls ) const;

    void WriteLoop();

    WString ColorizeWithSeverity( const WString &wstrText, ESeverity sev ) const;

  private:
    Mutex m_InstanceLock;

    Queue<LogStruct> m_MessageQueue;
    String           m_strTargetPath;
    String           m_strLogName;

    ABool        m_aIsWriteThreadWorking;
    Thread       m_tWriteThreadHandle;
    ConditionVar m_FlushCondition;

    UMap<WString, u32> m_Messages;
};

} // namespace B33::Core::Debug
#endif // !B33_LOGGER_H
