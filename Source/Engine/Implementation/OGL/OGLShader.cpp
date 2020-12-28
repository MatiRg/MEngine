#include "OGLShader.hpp"
#include "OGLTexture2D.hpp"
#include "../../Resources/Resources.hpp"
#include "../../System/System.hpp"
#include "../../Core/Log.hpp"
#include "../../Core/Utils.hpp"
#include <fstream>
#include <vector>
#include <array>
#include <sstream>

namespace
{
    EUniformType ToUniformType(const GLenum x)
    {
        switch (x)
        {
        case GL_INT:
            return EUniformType::Int;
        case GL_FLOAT:
            return EUniformType::Float;
        case GL_FLOAT_VEC2:
            return EUniformType::Vector2;
        case GL_FLOAT_VEC3:
            return EUniformType::Vector3;
        case GL_FLOAT_VEC4:
            return EUniformType::Vector4;
        case GL_FLOAT_MAT3:
            return EUniformType::Matrix3;
        case GL_FLOAT_MAT4:
            return EUniformType::Matrix4;
        case GL_SAMPLER_2D:
            return EUniformType::Texture;
        default:
            return EUniformType::Unknown;
        }
    }
}

COGLShader::COGLShader(const std::string& aName):
    IShader(aName)
{
    LOG( ESeverity::Debug ) << "OGL Shader - Created\n";
}

COGLShader::~COGLShader()
{
    glDeleteProgram( Handle );
    LOG( ESeverity::Debug ) << "OGL Shader - Destroyed\n";
}

bool COGLShader::ProcessFile(CResources* Resources, std::string& Data, std::stringstream& Buffer)
{
    for (std::string Line; std::getline(Buffer, Line); )
    {
        std::string Tmp = Utils::Trim(Line);
        if (Utils::StartWith(Tmp, "#include"))
        {
            std::size_t Start = Tmp.find("\"", 7u);
            std::size_t End = Tmp.find("\"", Start + 1);
            if (Start == End || Start == std::string::npos || End == std::string::npos)
            {
                LOG(ESeverity::Error) << "Invalid include command in Program Name - " << Path << "\n";
                return false;
            }
            Tmp = Tmp.substr(Start + 1, End - Start - 1);

            std::string IncludePath;
            if (!Resources->FindPath(Tmp, IncludePath))
            {
                LOG(ESeverity::Error) << "Invalid include File - Program Name - " << Path << "\n";
                return false;
            }

            std::stringstream IncBuffer;
            std::ifstream Stream(IncludePath, std::ifstream::in);
            if (Stream.is_open())
            {
                IncBuffer << Stream.rdbuf();
                Stream.close();
            }
            else
            {
                LOG(ESeverity::Error) << "Invalid include File - Program Name - " << Path << "\n";
                return false;
            }

            if ( !ProcessFile(Resources, Data, IncBuffer) )
            {
                return false;
            }
        }
        else
        {
            Data += Line + "\n";
        }
    }
    return true;
}

bool COGLShader::LoadProgram(CResources* Resources, GLuint Index, const StringVec& Defines)
{
    std::stringstream Buffer;
    std::ifstream Stream(Path, std::ifstream::in);
    if (Stream.is_open())
    {
        Buffer << Stream.rdbuf();
        Stream.close();
    }
    else
    {
        LOG(ESeverity::Error) << "No GPU Program with Name - " << Path << "\n";
        return false;
    }

    std::string Data;
    Data += "#version 460 core\n";

    for (const auto& i : Defines)
    {
        Data += "#define " + i + "\n";
    }

    if( !ProcessFile(Resources, Data, Buffer) )
    {
        return false;
    }

    GLint Result = 0;
    GLint InfoLength = 0;

    const char* Buffor1 = Data.c_str();
    glShaderSource(Index, 1, &Buffor1, 0);
    glCompileShader(Index);

    glGetShaderiv(Index, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(Index, GL_INFO_LOG_LENGTH, &InfoLength);
    if (Result == GL_FALSE)
    {
        std::vector<char> ErrorStr(static_cast<std::size_t>(InfoLength) + 1u, '\0');
        glGetShaderInfoLog(Index, InfoLength, nullptr, ErrorStr.data());
        LOG(ESeverity::Error) << ErrorStr.data() << "\n";
        return false;
    }

    return true;
}

#define DESTROY_PROGRAMS    glDeleteShader( VertexID ); \
    glDeleteShader(FragmentID)

bool COGLShader::Load(CResources* Resources, const ResourceCreateMap& VarMap)
{
    GLuint VertexID = glCreateShader( GL_VERTEX_SHADER );
    GLuint FragmentID = glCreateShader( GL_FRAGMENT_SHADER );

    StringVec BasicDefines = !VarMap.count(RESOURCES_VAR_DEFINES) ? StringVec() : std::any_cast<StringVec>(VarMap.at(RESOURCES_VAR_DEFINES));

    BasicDefines.push_back("VS");
    if (!LoadProgram(Resources, VertexID, BasicDefines))
    {
        DESTROY_PROGRAMS;
        return false;
    }
    BasicDefines.pop_back();

    BasicDefines.push_back("FS");
    if (!LoadProgram(Resources, FragmentID, BasicDefines))
    {
        DESTROY_PROGRAMS;
        return false;
    }
    BasicDefines.pop_back();

    GLint Result = 0;
    GLint InfoLength = 0;

    Handle = glCreateProgram();
    glAttachShader( Handle, VertexID );
    glAttachShader( Handle, FragmentID );
    glLinkProgram( Handle );

    glGetProgramiv( Handle, GL_LINK_STATUS, &Result );
    glGetProgramiv( Handle, GL_INFO_LOG_LENGTH, &InfoLength );
    if( Result == GL_FALSE )
    {
        std::vector<char> ErrorStr( static_cast<std::size_t>(InfoLength)+1u, '\0' );
        glGetProgramInfoLog( Handle, InfoLength, nullptr, ErrorStr.data() );
        LOG( ESeverity::Error )  << ErrorStr.data() << "\n";
        return false;
    }

    constexpr int MAX_NAME_LENGTH = 256;
    std::array<char, MAX_NAME_LENGTH> NameBuffer = {'\0'};
    int ElementCount;
    GLenum Type;
    // Check for uniforms and sampler units
    glGetProgramiv(Handle, GL_ACTIVE_UNIFORMS, &Result);
    for (int i = 0; i < Result; ++i)
    {
        glGetActiveUniform(Handle, static_cast<GLuint>(i), MAX_NAME_LENGTH, nullptr, &ElementCount, &Type, NameBuffer.data());
        std::string UniName = NameBuffer.data();
        GLint Location = glGetUniformLocation(Handle, UniName.c_str());
        UniformCache[UniName] = Location;
        Uniforms.emplace_back( UniName, ToUniformType(Type), ElementCount, ElementCount > 1 );
    }

    DESTROY_PROGRAMS;

    if( OGL::CheckErrorOpenGL() ) 
    {
        return false;
    }

    Valid = true;
    LOG( ESeverity::Info ) << "Loaded Shader - " << Path << " Handle - " << Handle << "\n";
    return true;
}

#undef DESTROY_PROGRAMS

void COGLShader::Bind()
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error )  << "Using Invalid Shader\n";
        return;
    }
    glUseProgram( Handle );
}

void COGLShader::UnBind()
{
}

GLint COGLShader::GetUniformIndex(const std::string& Name)
{
    auto It = UniformCache.find(Name);
    if (It != UniformCache.end())
    {
        return It->second;
    }
    else
    {
        GLint Uniform = glGetUniformLocation(Handle, Name.c_str());
        UniformCache[Name] = Uniform;
        return Uniform;
    }
}

void COGLShader::SetTexture(const std::string& Name, ITexture2D* aTexture, const int Index)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error )  << "Using Invalid Shader\n";
        return;
    }
    auto Texture = dynamic_cast<COGLTexture2D*>(aTexture);
    if( Texture )
    {
        glActiveTexture( GL_TEXTURE0+Index );
        glBindTexture( GL_TEXTURE_2D, Texture->GetHandle() );
        OGL::CheckErrorOpenGL();
        SetInteger( Name, Index );
    }
}

void COGLShader::SetInteger(const std::string& Name, const int v)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error )  << "Using Invalid Shader\n";
        return;
    }
    GLint Uniform = GetUniformIndex(Name);
    if( Uniform < 0 )
    {
        LOG( ESeverity::Error )  << "Invalid Uniform variable - " << Name << " In - " << GetName() << "\n";
    }
    else 
    {
        glUniform1i( Uniform, v );
    }
}

void COGLShader::SetFloat(const std::string& Name, const float v)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error )  << "Using Invalid Shader\n";
        return;
    }
    GLint Uniform = GetUniformIndex(Name);
    if( Uniform < 0 )
    {
        LOG( ESeverity::Error )  << "Invalid Uniform variable - " << Name << " In - " << GetName() << "\n";
    }
    else 
    {
        glUniform1f( Uniform, v );
    }
}

void COGLShader::SetVector2(const std::string& Name, const Vector2& v)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error )  << "Using Invalid Shader\n";
        return;
    }
    GLint Uniform = GetUniformIndex(Name);
    if( Uniform < 0 ) 
    {
        LOG( ESeverity::Error )  << "Invalid Uniform variable - " << Name << " In - " << GetName() << "\n";
    }
    else 
    {
        glUniform2f( Uniform, v.x, v.y );
    }
}

void COGLShader::SetVector3(const std::string& Name, const Vector3& v)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error )  << "Using Invalid Shader\n";
        return;
    }
    GLint Uniform = GetUniformIndex(Name);
    if( Uniform < 0 )
    {
        LOG( ESeverity::Error )  << "Invalid Uniform variable - " << Name << " In - " << GetName() << "\n";
    }
    else 
    {
        glUniform3f( Uniform, v.x, v.y, v.z );
    }
}

void COGLShader::SetVector4(const std::string& Name, const Vector4& v)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error )  << "Using Invalid Shader\n";
        return;
    }
    GLint Uniform = GetUniformIndex(Name);
    if( Uniform < 0 )
    {
        LOG( ESeverity::Error )  << "Invalid Uniform variable - " << Name << " In - " << GetName() << "\n";
    }
    else 
    {
        glUniform4f( Uniform, v.x, v.y, v.z, v.w);
    }
}

void COGLShader::SetColor(const std::string& Name, const Color& v)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error )  << "Using Invalid Shader\n";
        return;
    }
    GLint Uniform = GetUniformIndex(Name);
    if( Uniform < 0 )
    {
        LOG( ESeverity::Error )  << "Invalid Uniform variable - " << Name << " In - " << GetName() << "\n";
    }
    else 
    {
        glUniform4f( Uniform, v.GetR(), v.GetG(), v.GetB(), v.GetA() );
    }
}

void COGLShader::SetMatrix3(const std::string& Name, const Matrix3& v)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error )  << "Using Invalid Shader\n";
        return;
    }
    GLint Uniform = GetUniformIndex(Name);
    if( Uniform < 0 )
    {
        LOG( ESeverity::Error )  << "Invalid Uniform variable - " << Name << " In - " << GetName() << "\n";
    }
    else 
    {
        glUniformMatrix3fv( Uniform, 1, GL_FALSE, v.Data() );
    }
}

void COGLShader::SetMatrix4(const std::string& Name, const Matrix4& v)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error )  << "Using Invalid Shader\n";
        return;
    }
    GLint Uniform = GetUniformIndex(Name);
    if( Uniform < 0 )
    {
        LOG( ESeverity::Error )  << "Invalid Uniform variable - " << Name << " In - " << GetName() << "\n";
    }
    else 
    {
        glUniformMatrix4fv( Uniform, 1, GL_FALSE, v.Data() );
    }
}

