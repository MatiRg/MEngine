#include "Utils.hpp"
#include <cstdio>
#include <cstdarg>
#include <vector>
#include <algorithm>
#include <cmath>

namespace Utils
{
    std::string Trim(const std::string& x)
    {
        std::string s(x);

        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
        {
            return !std::isspace(ch);
        }));

        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
        {
            return !std::isspace(ch);
        }).base(), s.end());

        return s;
    }

    bool StartWith(const std::string& Value, const std::string& What)
    {
        return Value.find(What, 0u) == 0u;
    }

    std::string Format(const char* Fmt, ...)
    {
        if( !Fmt ) 
        {
            return "";
        }

        va_list List1;
        va_start( List1, Fmt );

        va_list List2;
        va_copy( List2, List1 );
        std::vector<char> Buff2( static_cast<std::size_t>(std::vsnprintf( nullptr, 0, Fmt, List1 ))+1u, '\0' );
        va_end( List1 );

        int Len = std::vsnprintf( Buff2.data(), Buff2.size(), Fmt, List2 );
        va_end( List2 );

        return ( Len <= 0 ) ? std::string() : std::string( Buff2.begin(), Buff2.begin()+Len );
    }

    std::string GetPath(const std::string& Path)
    {
        if( Path.empty() ) 
        {
            return "";
        }

        auto p = Path.find_last_of( "/\\" );
        if( p != std::string::npos )
        {
            return Path.substr( 0, p );
        }
        return "";
    }

    std::string GetNameFromPath(const std::string& Path)
    {
        if( Path.empty() ) 
        {
            return "";
        }

        auto p = Path.find_last_of( "/\\" );
        if( p != std::string::npos )
        {
            return Path.substr( p+1 );
        }
        return Path;
    }

    std::string GetNameFromExt(const std::string& Name)
    {
        if( Name.empty() ) 
        {
            return "";
        }

        auto p = Name.find_last_of( "." );
        if( p != std::string::npos )
        {
            return Name.substr( 0, p );
        }
        return Name;
    }

    std::string GetFileExt(const std::string& Name)
    {
        if( Name.empty() ) 
        {
            return "";
        }

        auto p = Name.find_last_of( "." );
        if( p != std::string::npos )
        {
            return Name.substr( p+1 );
        }
        return "";
    }

    std::string NormalizePath(const std::string& Path)
    {
        std::string Tmp = Path;
        for(char& i: Tmp)
        {
            if( i == '\\' )
            {
                i = '/';
            }
        }
        return Tmp;
    }

    std::string AddTrailingDirSeparator(const std::string& Path)
    {
        if (Path.empty())
        {
            return "/";
        }

        std::string Tmp = Path;
        char c = Tmp.back();
        if (c != '/' && c != '\\')
        {
            Tmp.push_back('/');
        }

        return Tmp;
    }

    std::string ToLower(const std::string& Text)
    {
        std::string Buffor = Text;
        std::transform( Buffor.begin(), Buffor.end(), Buffor.begin(), ::tolower );
        return Buffor;
    }

    std::string ToUpper(const std::string& Text)
    {
        std::string Buffor = Text;
        std::transform( Buffor.begin(), Buffor.end(), Buffor.begin(), ::toupper );
        return Buffor;
    }

    std::vector<std::string> Split(const std::string& Text, const char D)
    {
        std::vector<std::string> Tmp;
        std::string::size_type Pos = Text.find(D), FPos = 0;
        while( Pos != std::string::npos )
        {
            Tmp.push_back( Text.substr(FPos, Pos-FPos) );
            FPos = Pos+1;
            Pos = Text.find(D, FPos);
        }
        Tmp.push_back( Text.substr( FPos ) );
        return Tmp;
    }

    std::string ToUTF8(const std::u16string& Str)
    {
        std::string Tmp;
        for(std::size_t i = 0; i < Str.size(); ++i)
        {
            if( Str[i] < 0x80 )
            {
                Tmp.push_back( Str[i] );
            }
            else if( Str[i] < 0x0800 )
            {
                Tmp.push_back( (Str[i] >> 6) | 0xC0 ); // 110xxxxx
                Tmp.push_back( (Str[i] & 0x3F) | 0x80 ); // 10xxxxxx
            }
        }
        return Tmp;
    }

    std::u16string ToUTF16(const std::string& Str)
    {
        std::u16string Tmp;
        for(std::size_t i = 0; i < Str.size(); ++i)
        {
            char16_t x = Str[i] & 0xFF; // masking
            if( x < 0x80 ) // 1
            {
                Tmp.push_back( Str[i] );
            }
            else if( (x >> 5) == 0x6 ) // 2, 110xxxxx
            {
                x = ( (x << 6) & 0x7ff ) + (Str[++i] & 0x3f);
                Tmp.push_back( x );
            }
        }
        return Tmp;
    }

    bool IsInteger(const std::string& Str)
    {
        if( Str.empty() ) 
        {
            return false;
        }

        std::string::const_iterator It = Str.begin();
        if( Str.size() > 1 && (Str[0] == '-' || Str[0] == '+') )
        {
            ++It;
        }
        return std::all_of(It, Str.end(), ::isdigit);
    }

    int GetHexDigit(char x, const int p)
    {
        x = tolower( x );
        int x1 = x >= '0' && x <= '9' ? x - '0' : x >= 'a' && x <= 'f' ? (x - 'a') + 10 : 0;
        return x1 * std::pow( 16, p );
    }
}
