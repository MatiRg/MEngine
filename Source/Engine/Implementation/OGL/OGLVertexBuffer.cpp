#include "OGLVertexBuffer.hpp"
#include "../../Core/Log.hpp"

namespace
{
    GLenum ToGLPrimitiveMode(const EPrimitiveMode x)
    {
        switch(x)
        {
        case EPrimitiveMode::Triangles:
            return GL_TRIANGLES;
        case EPrimitiveMode::Lines:
            return GL_LINES;
        case EPrimitiveMode::Points:
            return GL_POINTS;
        default:
            return GL_TRIANGLES;
        }
    }
}

COGLVertexBufferColorTexCoords::COGLVertexBufferColorTexCoords(const bool Dynamic): 
    IVertexBufferColorTexCoords(Dynamic)
{
    LOG( ESeverity::Debug ) << "OGL Vertex Buffer - Created\n";
}

COGLVertexBufferColorTexCoords::~COGLVertexBufferColorTexCoords()
{
    glDeleteBuffers( 1, &Coords );
    glDeleteBuffers( 1, &Color );
    glDeleteBuffers( 1, &Position );
    glDeleteVertexArrays( 1, &VAO );
    LOG( ESeverity::Debug ) << "OGL Vertex Buffer - Destroyed\n";
}

bool COGLVertexBufferColorTexCoords::Create()
{
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &Position );
    glGenBuffers( 1, &Color );
    glGenBuffers( 1, &Coords );

    glBindVertexArray( VAO );

    glBindBuffer( GL_ARRAY_BUFFER, Position );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), nullptr );
    glEnableVertexAttribArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, Color );
    glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, sizeof(::Color), nullptr );
    glEnableVertexAttribArray( 1 );

    glBindBuffer( GL_ARRAY_BUFFER, Coords );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), nullptr );
    glEnableVertexAttribArray( 2 );

    glBindVertexArray( 0 );

    if( OGL::CheckErrorOpenGL() ) 
    {
        return false;
    }

    Valid = true;
    return true;
}

void COGLVertexBufferColorTexCoords::Bind()
{
    if( IsValid() ) 
    {
        glBindVertexArray( VAO );
    }
}

void COGLVertexBufferColorTexCoords::Draw(const EPrimitiveMode Mode, const std::size_t Size)
{
    if( IsValid() ) 
    {
        glDrawArrays( ToGLPrimitiveMode(Mode), 0, static_cast<GLsizei>(Size) );
        OGL::CheckErrorOpenGL();
    }
}

void COGLVertexBufferColorTexCoords::UnBind()
{
}

void COGLVertexBufferColorTexCoords::FeedPositions(const std::vector<Vector3>& Data)
{
    if( IsValid() )
    {
        glBindBuffer( GL_ARRAY_BUFFER, Position );
        glBufferData( GL_ARRAY_BUFFER, Data.size()*sizeof(Vector3), Data.data(), IsDynamic() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW );
        OGL::CheckErrorOpenGL();
    }
}

void COGLVertexBufferColorTexCoords::FeedColors(const std::vector<::Color>& Data)
{
    if( IsValid() )
    {
        glBindBuffer( GL_ARRAY_BUFFER, Color );
        glBufferData( GL_ARRAY_BUFFER, Data.size()*sizeof(::Color), Data.data(), IsDynamic() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW );
        OGL::CheckErrorOpenGL();
    }
}

void COGLVertexBufferColorTexCoords::FeedTextureCoords(const std::vector<Vector2>& Data)
{
    if( IsValid() )
    {
        glBindBuffer( GL_ARRAY_BUFFER, Coords );
        glBufferData( GL_ARRAY_BUFFER, Data.size()*sizeof(Vector2), Data.data(), IsDynamic() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW );
        OGL::CheckErrorOpenGL();
    }
}