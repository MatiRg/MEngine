#include "Log.hpp"
#include <cstdio>
#include <cstdarg>
#include <algorithm>
#include <iostream>

CLog::CLog()
{
    Stream = std::make_unique<std::ostream>( &Buffor );
}

CLog::~CLog()
{
}

CLog& CLog::GetInstance()
{
    static CLog Log;
    return Log;
}

CLog& CLog::AddSink(const std::shared_ptr<ISink>& Sink)
{
    Buffor.AddSink( Sink );
    return *this;
}

CLog& CLog::operator<<(const ESeverity x)
{
    switch( x )
    {
    case ESeverity::Verbose:
        return operator<<( "[Verbose] " );
    case ESeverity::Debug:
        return operator<<( "[Debug] " );
    case ESeverity::Info:
        return operator<<( "[Info] " );
    case ESeverity::Warning:
        return operator<<( "[Warning] " );
    case ESeverity::Error:
        return operator<<( "[Error] " );
    case ESeverity::Fatal:
        return operator<<( "[Fatal] " );
    case ESeverity::Script:
        return operator<<( "[Script] " );
    default:
        return *this;
    }
}

CLog& CLog::Write(const char* Fmt, ...)
{
    if( !Fmt ) 
    {
        return *this;
    }

    va_list List1;
    va_start( List1, Fmt );

    va_list List2;
    va_copy( List2, List1 );
    std::vector<char> Buff2( std::vsnprintf( nullptr, 0, Fmt, List1 )+1, '\0' );
    va_end( List1 );

    int Len = std::vsnprintf( Buff2.data(), Buff2.size(), Fmt, List2 );
    va_end( List2 );

    if( Len <= 0 ) 
    {
        return *this;
    }

    return operator<<( std::string( Buff2.begin(), Buff2.begin()+Len ) );
}

CLog& CLog::Write(const ESeverity x)
{
    return operator<<( x );
}

//

CLog::CStreamBuffor::int_type CLog::CStreamBuffor::overflow(CLog::CStreamBuffor::int_type Char)
{
    for(auto& i: Sinks)
    {
        if( i && i->IsValid() )
        {
            i->Append( {static_cast<char>(Char)} );
        }
    }
    return 1;
}

void CLog::CStreamBuffor::AddSink(const std::shared_ptr<ISink>& Sink)
{
    if( Sink )
    {
        Sinks.push_back( Sink );
    }
}

//

void CStdSink::Append(const std::string& Text)
{
    std::cout << Text;
}

bool CStdSink::IsValid() const
{
    return std::cout.good();
}

//

CFileSink::CFileSink(const std::string& File):
    Stream( File )
{
}

void CFileSink::Append(const std::string& Text)
{
    Stream << Text;
}

bool CFileSink::IsValid() const
{
    return Stream.is_open() && Stream.good();
}