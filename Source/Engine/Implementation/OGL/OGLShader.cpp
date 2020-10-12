#include "OGLShader.hpp"
#include "OGLTexture2D.hpp"
#include "../../Resources/Resources.hpp"
#include "../../System/System.hpp"
#include "../../Core/Log.hpp"
#include "../../Core/Utils.hpp"
#include <fstream>
#include <vector>

COGLShader::COGLShader(const std::string& aName, CResources* aResources):
    IShader(aName, aResources)
{
    LOG( ESeverity::Debug ) << "OGL Shader - Created\n";
}

COGLShader::~COGLShader()
{
    glDeleteProgram( Handle );
    LOG( ESeverity::Debug ) << "OGL Shader - Destroyed\n";
}

bool COGLShader::Load()
{
    GLuint VertexID = glCreateShader( GL_VERTEX_SHADER );
    GLuint FragmentID = glCreateShader( GL_FRAGMENT_SHADER );

    std::string VSFile = Path+".vs";
    std::string FSFile = Path+".fs";

    std::string VertexBuffer;
    std::ifstream VertexStream( VSFile, std::ifstream::in );
    if( VertexStream.is_open() )
    {
        VertexBuffer = std::string( (std::istreambuf_iterator<char>(VertexStream)), std::istreambuf_iterator<char>() );
        VertexStream.close();
    }
    else
    {
        LOG( ESeverity::Error ) << "No Vertex Shader with Name - " << VSFile << "\n";
        return false;
    }

    std::string FragmentBuffer;
    std::ifstream FragmentStream( FSFile, std::ifstream::in );
    if( FragmentStream.is_open() )
    {
        FragmentBuffer = std::string( (std::istreambuf_iterator<char>(FragmentStream)), std::istreambuf_iterator<char>() );
        FragmentStream.close();
    }
    else
    {
        LOG( ESeverity::Error ) << "No Fragment Shader with Name - " << FSFile << "\n";
        return false;
    }

    GLint Result = 0;
    GLint InfoLength = 0;

    const char* Buffor1 = VertexBuffer.c_str();
    glShaderSource( VertexID, 1, &Buffor1, 0 );
    glCompileShader( VertexID );

    glGetShaderiv( VertexID, GL_COMPILE_STATUS, &Result );
    glGetShaderiv( VertexID, GL_INFO_LOG_LENGTH, &InfoLength );
    if( Result == GL_FALSE )
    {
        std::vector<char> ErrorStr( InfoLength+1, '\0' );
        glGetShaderInfoLog( VertexID, InfoLength, nullptr, ErrorStr.data() );
        LOG( ESeverity::Error ) << ErrorStr.data() << "\n";
        return false;
    }

    const char* Buffor2 = FragmentBuffer.c_str();
    glShaderSource( FragmentID, 1, &Buffor2, 0 );
    glCompileShader( FragmentID );

    glGetShaderiv( FragmentID, GL_COMPILE_STATUS, &Result );
    glGetShaderiv( FragmentID, GL_INFO_LOG_LENGTH, &InfoLength );
    if( Result == GL_FALSE )
    {
        std::vector<char> ErrorStr( InfoLength+1, '\0' );
        glGetShaderInfoLog( FragmentID, InfoLength, nullptr, ErrorStr.data() );
        LOG( ESeverity::Error )  << ErrorStr.data() << "\n";
        return false;
    }

    Handle = glCreateProgram();
    glAttachShader( Handle, VertexID );
    glAttachShader( Handle, FragmentID );
    glLinkProgram( Handle );

    glGetProgramiv( Handle, GL_LINK_STATUS, &Result );
    glGetProgramiv( Handle, GL_INFO_LOG_LENGTH, &InfoLength );
    if( Result == GL_FALSE )
    {
        std::vector<char> ErrorStr( InfoLength+1, '\0' );
        glGetProgramInfoLog( Handle, InfoLength, nullptr, ErrorStr.data() );
        LOG( ESeverity::Error )  << ErrorStr.data() << "\n";
        return false;
    }

    glDeleteShader( VertexID );
    glDeleteShader( FragmentID );

    if( OGL::CheckErrorOpenGL() ) 
    {
        return false;
    }

    Valid = true;
    LOG( ESeverity::Info ) << "Loaded Shader - " << Path << " Handle - " << Handle << "\n";
    return true;
}

bool COGLShader::Find()
{
    bool FoundPath;
    std::string TmpPath;
    std::tie(FoundPath, TmpPath) = Resources->FindPath(GetName()+".vs");
    if( !FoundPath )
    {
        LOG( ESeverity::Error ) << "Unable to Find Vertex Program Path: " << GetName()+".vs" << "\n";
        return false;
    }
    Path = Utils::GetNameFromExt(TmpPath); // Path is preserved
    if( !Resources->GetSystem()->FileExist( Path+".fs" ) ) // Fragment must exist in same directory
    {
        LOG( ESeverity::Error ) << "Unable to Find Fragment Program Path: " << GetName()+".fs" << "\n";
        return false;
    }
    return true;
}

bool COGLShader::Exist()
{
    std::string VSFile = Path+".vs";
    std::string FSFile = Path+".fs";
    if( !Resources->GetSystem()->FileExist( VSFile ) )
    {
        return false;
    }
    if( !Resources->GetSystem()->FileExist( FSFile ) )
    {
        return false;
    }
    return true;
}

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

void COGLShader::SetTexture(ITexture2D* aTexture, const std::string& Name, const int Index)
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
    GLint Uniform = glGetUniformLocation( Handle, Name.c_str() );
    if( Uniform < 0 )
    {
        LOG( ESeverity::Error )  << "Invalid Uniform variable - " << Name << " In - " << FileName << "\n";
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
    GLint Uniform = glGetUniformLocation( Handle, Name.c_str() );
    if( Uniform < 0 )
    {
        LOG( ESeverity::Error )  << "Invalid Uniform variable - " << Name << " In - " << FileName << "\n";
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
    GLint Uniform = glGetUniformLocation( Handle, Name.c_str() );
    if( Uniform < 0 ) 
    {
        LOG( ESeverity::Error )  << "Invalid Uniform variable - " << Name << " In - " << FileName << "\n";
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
    GLint Uniform = glGetUniformLocation( Handle, Name.c_str() );
    if( Uniform < 0 )
    {
        LOG( ESeverity::Error )  << "Invalid Uniform variable - " << Name << " In - " << FileName << "\n";
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
    GLint Uniform = glGetUniformLocation( Handle, Name.c_str() );
    if( Uniform < 0 )
    {
        LOG( ESeverity::Error )  << "Invalid Uniform variable - " << Name << " In - " << FileName << "\n";
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
    GLint Uniform = glGetUniformLocation( Handle, Name.c_str() );
    if( Uniform < 0 )
    {
        LOG( ESeverity::Error )  << "Invalid Uniform variable - " << Name << " In - " << FileName << "\n";
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
    GLint Uniform = glGetUniformLocation( Handle, Name.c_str() );
    if( Uniform < 0 )
    {
        LOG( ESeverity::Error )  << "Invalid Uniform variable - " << Name << " In - " << FileName << "\n";
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
    GLint Uniform = glGetUniformLocation( Handle, Name.c_str() );
    if( Uniform < 0 )
    {
        LOG( ESeverity::Error )  << "Invalid Uniform variable - " << Name << " In - " << FileName << "\n";
    }
    else 
    {
        glUniformMatrix4fv( Uniform, 1, GL_FALSE, v.Data() );
    }
}

