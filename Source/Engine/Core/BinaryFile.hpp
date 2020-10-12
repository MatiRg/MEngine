#pragma once
#include <string>
#include <cstdio>

enum class EFileMode
{
    Read,
    Write
};

enum class EFileSeek
{
    Begin,
    Current,
    End
};

class CBinaryFile final
{
public:
    CBinaryFile()
    {
    }

    CBinaryFile(const std::string& File, const EFileMode Mode)
    {
        Open( File, Mode );
    }

    CBinaryFile(CBinaryFile&& Other):
        Stream( Other.Stream )
    {
        Other.Stream = nullptr;
    }

    ~CBinaryFile()
    {
        Close();
    }

    CBinaryFile& operator=(CBinaryFile&& Other)
    {
        Stream = Other.Stream;
        Other.Stream = nullptr;
        return *this;
    }

    bool Open(const std::string& File, const EFileMode Mode)
    {
        Close();
        Stream = std::fopen( File.c_str(), Mode == EFileMode::Read ? "rb" : "wb" );
        return Stream;
    }

    void Close()
    {
        if( Stream )
        {
            std::fclose( Stream );
            Stream = nullptr;
        }
    }

    bool IsOpen() const { return Stream; }
    operator bool() const { return IsOpen(); }

    int Tell() const { return std::ftell( Stream ); }

    bool Seek(const int Offset, EFileSeek Mode)
    {
        int Origin = SEEK_SET;
        if( Mode == EFileSeek::Current ) Origin = SEEK_CUR;
        if( Mode == EFileSeek::End ) Origin = SEEK_END;
        return std::fseek( Stream, Offset, Origin ) == 0;
    }

    template<class T>
    std::size_t Read(T& Value)
    {
        return Read( &Value, sizeof(T), 1 );
    }

    template<class T>
    std::size_t Write(const T& Value)
    {
        return Write( &Value, sizeof(T), 1 );
    }

    std::size_t Read(void* Ptr, const std::size_t Size, const std::size_t Count)
    {
        return std::fread( Ptr, Size, Count, Stream );
    }

    std::size_t Write(const void* Ptr, const std::size_t Size, const std::size_t Count)
    {
        return std::fwrite( Ptr, Size, Count, Stream );
    }

    CBinaryFile(const CBinaryFile&) = delete;
    CBinaryFile& operator=(const CBinaryFile&) = delete;
private:
    std::FILE* Stream = nullptr;
};
