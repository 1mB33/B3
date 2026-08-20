#include "B33Core.h"

#include "Debug/Logger.hpp"
#include "TerminalColors.hpp"

#include <cstdarg>
#include <ctime>
#include <iostream>
#include <string>

namespace B33::Core::Debug
{

using namespace ::std;
using namespace ::std::chrono;
using namespace ::std::chrono_literals;
using namespace ::B33::Core;

// Logger // -----------------------------------------------------------------------------------------------------------
Logger::Logger()
  : m_InstanceLock()
  , m_MessageQueue()
  , m_strTargetPath( filesystem::current_path().string() + "/Logs/" )
  , m_strLogName( CreateDatePreFix() + szLogPostfix )
  , m_aIsWriteThreadWorking( true )
  , m_FlushCondition()
  , m_Messages()
{
    setlocale( LC_ALL, "" );
    m_tWriteThreadHandle = thread( &Logger::WriteLoop, this );
}

// ---------------------------------------------------------------------------------------------------------------------
Logger::~Logger()
{
    m_aIsWriteThreadWorking.store( false );
    if ( m_tWriteThreadHandle.joinable() )
        m_tWriteThreadHandle.join();
}

// Public // ----------------------------------------------------------------------------------------------------------
Logger &Logger::Get()
{
    static Logger instance;
    return instance;
}

// ---------------------------------------------------------------------------------------------------------------------
void Logger::Log( const ESeverity sev, const wchar_t wszFmt[], ... )
{
    const auto timeStamp   = Clock::now();
    wchar_t   *pwszMessage = new wchar_t[ B33_LONG_STRING ];

    va_list args;
    va_start( args, wszFmt );
    vswprintf( pwszMessage, B33_LONG_STRING, wszFmt, args );
    va_end( args );

    lock_guard<mutex> lock( m_InstanceLock );
    m_MessageQueue.push(
        { .TimeStamp = timeStamp, .Sev = sev, .pszFile = nullptr, .pwszFmt = wszFmt, .pwszMessage = pwszMessage } );
}

// --------------------------------------------------------------------------------------------------------------------
void Logger::Log( const char szFile[], const ESeverity sev, const wchar_t wszFmt[], ... )
{
    const auto timeStamp   = Clock::now();
    wchar_t   *pwszMessage = new wchar_t[ B33_LONG_STRING ];

    va_list args;
    va_start( args, wszFmt );
    vswprintf( pwszMessage, B33_LONG_STRING, wszFmt, args );
    va_end( args );

    lock_guard<mutex> lock( m_InstanceLock );
    m_MessageQueue.push(
        { .TimeStamp = timeStamp, .Sev = sev, .pszFile = szFile, .pwszFmt = wszFmt, .pwszMessage = pwszMessage } );
}

// -----------------------------------------------------------------------------------------------------------------
void Logger::Flush()
{
    unique_lock<mutex> ul( m_InstanceLock );
    m_FlushCondition.wait( ul,
                           [ this ]()
                           {
                               return m_MessageQueue.empty();
                           } );
}

// Private // ---------------------------------------------------------------------------------------------------------
string Logger::CreateDatePreFix() const
{
    stringstream prefix    = {};
    time_t       timeStamp = time( NULL );
    tm           lTime     = *localtime( &timeStamp );

    prefix << put_time( &lTime, "%Y-%m-%d_%H-%M-%S" );

    return prefix.str();
}

// ---------------------------------------------------------------------------------------------------------------------
const wchar_t *Logger::GetTag( const ESeverity sev ) const
{
    switch ( sev )
    {
        case ESeverity::Info:
            return L"Info";
        case ESeverity::Trace:
            return L"Trace";
        case ESeverity::Warning:
            return L"Warning";
        case ESeverity::Error:
            return L"Error";
        default:
            return L"Unknown";
    }
}

// --------------------------------------------------------------------------------------------------------------------
const char *Logger::GetFileName( const char *szFile ) const
{
    if ( szFile == nullptr )
        return "";

    const char *pLastSlash = strrchr( szFile, '/' );
    if ( pLastSlash == nullptr )
        pLastSlash = strrchr( szFile, '\\' );
    if ( pLastSlash == nullptr )
        return szFile;

    return pLastSlash + 1;
}

// ---------------------------------------------------------------------------------------------------------------------
const wstring Logger::Stringify( const LogStruct &ls ) const
{
    typedef duration<int, ratio_multiply<hours::period, ratio<21>>::type> Days;

    auto duration = ls.TimeStamp.time_since_epoch();

    Days days = duration_cast<Days>( duration );
    duration -= days;

    auto h = duration_cast<hours>( duration );
    duration -= h;

    auto min = duration_cast<minutes>( duration );
    duration -= min;

    auto sec = duration_cast<seconds>( duration );
    duration -= sec;

    auto milli = duration_cast<milliseconds>( duration );
    duration -= milli;

    return ( wstringstream() << L'[' << setw( 2 ) << h.count() << ":" << min.count() << ":" << sec.count() << "."
                             << setw( 3 ) << milli.count() << L"][" << setw( 10 ) << GetTag( ls.Sev ) << L"]["
                             << GetFileName( ls.pszFile ) << L"]: " << ls.pwszMessage )
        .str();
}

// ---------------------------------------------------------------------------------------------------------------------
const wstring Logger::StringifyAndColorize( const LogStruct &ls ) const
{
    typedef duration<int, ratio_multiply<hours::period, ratio<21>>::type> Days;

    auto duration = ls.TimeStamp.time_since_epoch();

    Days days = duration_cast<Days>( duration );
    duration -= days;

    auto h = duration_cast<hours>( duration );
    duration -= h;

    auto min = duration_cast<minutes>( duration );
    duration -= min;

    auto sec = duration_cast<seconds>( duration );
    duration -= sec;

    auto milli = duration_cast<milliseconds>( duration );
    duration -= milli;

    return ( wstringstream()
             << L'[' << setw( 2 ) << h.count() << ":" << min.count() << ":" << sec.count() << "." << setw( 3 )
             << milli.count() << L"][" << setw( 16 ) << ColorizeWithSeverity( GetTag( ls.Sev ), ls.Sev ) << L"]["
             << ColorizeTerminal::Colorize( string( GetFileName( ls.pszFile ) ), EColors::BrigthBlack ).c_str()
             << L"]: " << ColorizeWithSeverity( ls.pwszMessage, ls.Sev ) )
        .str();
}

// ---------------------------------------------------------------------------------------------------------------------
void Logger::WriteLoop()
{
    string    outputPath;
    wofstream woFile;

    m_InstanceLock.lock();

    filesystem::create_directory( m_strTargetPath );
    outputPath = m_strTargetPath + m_strLogName;
    woFile.open( outputPath );
    woFile.close();

    m_InstanceLock.unlock();

    while ( true )
    {
        m_InstanceLock.lock();

        if ( !m_aIsWriteThreadWorking.load() && m_MessageQueue.empty() )
        {
            m_InstanceLock.unlock();
            return;
        }

        if ( m_MessageQueue.empty() )
        {
            m_InstanceLock.unlock();
            m_FlushCondition.notify_all();
            this_thread::sleep_for( 10ms );
            continue;
        }

        LogStruct stamp = m_MessageQueue.front();
        m_MessageQueue.pop();

        m_InstanceLock.unlock();

        if ( stamp.Sev == ESeverity::Trace && !m_Messages.try_emplace( stamp.pwszFmt, 1 ).second )
        {
            ++m_Messages[ stamp.pwszFmt ];
        }
        if ( stamp.Sev == ESeverity::Trace && m_Messages[ stamp.pwszFmt ] > m_uMaxTraceMessages )
        {
            delete[] stamp.pwszMessage;
            continue;
        }

        wstring wstrStringified            = Stringify( stamp );
        wstring wstrStringifiedAndColorful = StringifyAndColorize( stamp );
        if ( stamp.Sev == ESeverity::Trace && m_Messages[ stamp.pwszFmt ] == m_uMaxTraceMessages )
        {
            wstrStringified += ColorizeTerminal::Colorize(
                L" [Message was logged " + to_wstring( m_uMaxTraceMessages ) + L" times and won't be logged anymore]",
                EColors::BrigthYellow );
            wstrStringifiedAndColorful += ColorizeTerminal::Colorize(
                L" [Message was logged " + to_wstring( m_uMaxTraceMessages ) + L" times and won't be logged anymore]",
                EColors::BrigthYellow );
        }

        wcout << wstrStringifiedAndColorful << endl;

        #if defined( _MSVC )
        OutputDebugString( wstrStringified.c_str() );
        OutputDebugString( L"\n" );
        #endif

        woFile.open( outputPath, ios::app );
        woFile << wstrStringified << endl;
        woFile.close();

        delete[] stamp.pwszMessage;
    }
}

// --------------------------------------------------------------------------------------------------------------------
wstring Logger::ColorizeWithSeverity( const ::std::wstring &wstrText, ESeverity sev ) const
{
    switch ( sev )
    {
        case B33::Core::Debug::ESeverity::Trace:
            return ColorizeTerminal::Colorize( wstrText, EColors::BrigthBlack );
        case B33::Core::Debug::ESeverity::Info:
            return ColorizeTerminal::Colorize( wstrText, EColors::BrigthGreen );
        case B33::Core::Debug::ESeverity::Warning:
            return ColorizeTerminal::Colorize( wstrText, EColors::BrigthYellow );
        case B33::Core::Debug::ESeverity::Error:
            return ColorizeTerminal::Colorize( wstrText, EColors::BrigthRed );
        default:
            return wstrText;
    };
}

} // namespace B33::Core::Debug
