#pragma once
#include "NonCopyable.hpp"
#include <ostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>

// Synchronized with C# counter part - Severity
enum class ESeverity
{
    Verbose = 0, /// Everything
    Debug   = 1,
    Info    = 2,
    Warning = 3,
    Error   = 4,
    Fatal   = 5,
    Script  = 6
};

class ISink: public NonCopyableMovable 
{
public:
    ISink() = default;
    virtual ~ISink() = default;

    virtual void Append(const std::string&) = 0;
    virtual bool IsValid() const = 0;
};

// Log data to all sinks
class CLog final: public NonCopyableMovable
{
public:
    ~CLog();

    static CLog& GetInstance();

    CLog& AddSink(const std::shared_ptr<ISink>&);

    ESeverity GetSeverity() const { return Severity; }
    void SetSeverity(const ESeverity Sev) { Severity = Sev; }

    CLog& operator<<(const ESeverity);

    template<class T>
    CLog& operator<<(const T& Value)
    {
        if( Stream->good() )
        {
            (*Stream) << Value;
        }
        return *this;
    }

    CLog& Write(const char*, ...);
    CLog& Write(const ESeverity);
private:
    class CStreamBuffor final: public std::streambuf
    {
    public:
        int_type overflow(int_type) override;

        void AddSink(const std::shared_ptr<ISink>&);
    private:
        std::vector<std::shared_ptr<ISink>> Sinks;
    };
private:
    CLog();
private:
    ESeverity Severity = ESeverity::Verbose;
    CStreamBuffor Buffor;
    std::unique_ptr<std::ostream> Stream;
};

inline CLog& GetLog()
{
    return CLog::GetInstance();
}

#define LOG(Severity) if( Severity >= GetLog().GetSeverity() ) GetLog().Write( Severity )
#define LOG_IF(Severity, Condition) if( Condition ) LOG( Severity )

//

class CStdSink final: public ISink
{
public:
    void Append(const std::string&) override;
    bool IsValid() const override;
};

class CFileSink final: public ISink
{
public:
    CFileSink(const std::string&);

    void Append(const std::string&) override;
    bool IsValid() const override;
private:
    std::ofstream Stream;
};