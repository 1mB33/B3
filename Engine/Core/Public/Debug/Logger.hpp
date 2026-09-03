#if !defined(B33_LOGGER_H)
#define B33_LOGGER_H

#include <string>

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

    struct LogStruct
    {
        ClockTimePoint TimeStamp;
        ESeverity      Sev;
        const char    *pszFile;
        const wchar_t *pwszFmt;
        wchar_t       *pwszMessage;
    };

    const ::uint32_t MAX_TRACE_MESSAGES = 15;

    __B33_API Logger();

    Logger( const Logger & ) = delete;
    Logger( Logger && )      = delete;

    Logger &operator=( const Logger & ) = delete;
    Logger &operator=( Logger && )      = delete;

  public:
    __B33_API static Logger &Get();

    __B33_API ~Logger();

  public:
    /**
     * @brief Logs message to the file.
     *
     * @param sev Severity tag of the log message
     * @param pwszFmt Format for the message or message itself, written with `vswprintf` standard
     * @param ... Variables to be used in the format
     */
    __B33_API void Log( const ESeverity sev, const wchar_t wszFmt[], ... );

    /**
     * @brief Logs message to the file.
     *
     * @param file Name of file from which the message come
     * @param sev Severity tag of the log message
     * @param pwszFmt Format for the message or message itself, written with `vswprintf` standard
     * @param ... Variables to be used in the format
     */
    __B33_API void Log( const char szFile[], const ESeverity sev, const wchar_t wszFmt[], ... );

    __B33_API void Flush();

  private:
    ::std::string CreateDatePreFix() const;

    const wchar_t *GetTag( const ESeverity sev ) const;

    const char *GetFileName( const char *szFile ) const;

    const ::std::wstring Stringify( const LogStruct &ls ) const;

    const ::std::wstring StringifyAndColorize( const LogStruct &ls ) const;

    void WriteLoop();

    ::std::wstring ColorizeWithSeverity( const ::std::wstring &wstrText, ESeverity sev ) const;

  private:
    ::std::mutex m_InstanceLock;

    ::std::queue<LogStruct> m_MessageQueue;
    ::std::string           m_strTargetPath;
    ::std::string           m_strLogName;

    ::std::atomic_bool        m_aIsWriteThreadWorking;
    ::std::thread             m_tWriteThreadHandle;
    ::std::condition_variable m_FlushCondition;

    ::std::unordered_map<::std::wstring, ::uint32_t> m_Messages;
};

} // namespace B33::Core::Debug
#endif // !B33_LOGGER_H
