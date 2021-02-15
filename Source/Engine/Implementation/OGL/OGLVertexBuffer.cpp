#include "OGLVertexBuffer.hpp"
#include "../../Core/Log.hpp"

namespace
{
    GLenum ToGLPrimitiveMode(const EPrimitiveType x)
    {
        switch(x)
        {
        case EPrimitiveType::Triangles:
            return GL_TRIANGLES;
        case EPrimitiveType::Lines:
            return GL_LINES;
        case EPrimitiveType::Points:
            return GL_POINTS;
        default:
            return GL_TRIANGLES;
        }
    }
}

COGLVertexBuffer::COGLVertexBuffer()
{
    LOG( ESeverity::Debug ) << "OGL Vertex Buffer - Created\n";
}

COGLVertexBuffer::~COGLVertexBuffer()
{
    if( HasEBO )
    {
        glDeleteBuffers(1, &EBO);
    }
    for (const auto& i : VBOs)
    {
        glDeleteBuffers(1, &i.Index);
    }
    glDeleteVertexArrays( 1, &VAO );
    LOG( ESeverity::Debug ) << "OGL Vertex Buffer - Destroyed\n";
}

bool COGLVertexBuffer::Create(const std::vector<SVertexElement>& aDescriptors, const bool aDynamic)
{
    if( aDescriptors.empty() )
    {
        return false;
    }

    Dynamic = aDynamic;

    glGenVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );
    for (const auto& i : aDescriptors)
    {
        GLuint Index = 0u;
        glGenBuffers(1, &Index);

        VBOs.push_back( {Index, i} );

        glBindBuffer(GL_ARRAY_BUFFER, Index);

        glVertexAttribPointer(static_cast<GLuint>(i.Element), i.ElementSize, GL_FLOAT, GL_FALSE, i.Size, nullptr);
        glEnableVertexAttribArray(static_cast<GLuint>(i.Element));
    }
    glBindVertexArray( 0 );

    if( OGL::CheckErrorOpenGL() ) 
    {
        return false;
    }

    Valid = true;
    return true;
}

bool COGLVertexBuffer::HasElement(const EVertexElement Element) const
{
    for (const auto& i : VBOs)
    {
        if (i.Descriptor.Element == Element)
        {
            return true;
        }
    }
    return false;
}

const COGLVertexBuffer::SVBO* COGLVertexBuffer::GetVBO(const EVertexElement Element)
{
    for (const auto& i : VBOs)
    {
        if (i.Descriptor.Element == Element)
        {
            return &i;
        }
    }
    return nullptr;
}

void COGLVertexBuffer::Bind()
{
    if( IsValid() ) 
    {
        glBindVertexArray( VAO );
    }
}

void COGLVertexBuffer::Draw(const EPrimitiveType Mode)
{
    Draw(Mode, HasEBO ? IndicesCount : VertexCount);
}

void COGLVertexBuffer::Draw(const EPrimitiveType Mode, const std::size_t Size)
{
    if( IsValid() )
    {
        if( HasEBO )
        {
            glDrawElements(ToGLPrimitiveMode(Mode), static_cast<GLsizei>(Size), GL_UNSIGNED_INT, nullptr);
        }
        else
        {
            glDrawArrays(ToGLPrimitiveMode(Mode), 0, static_cast<GLsizei>(Size));
        }
        OGL::CheckErrorOpenGL();
    }
}

void COGLVertexBuffer::UnBind()
{
}

void COGLVertexBuffer::SetData(const EVertexElement Type, const std::vector<Vector3>& Data)
{
    if (IsValid())
    {
        const SVBO* VBO = GetVBO(Type);
        if( !VBO )
        {
            LOG(ESeverity::Error) << "Vertex Buffer Do Not Support this Vertex Type\n";
            return;
        }
        if( Data.empty() )
        {
            LOG(ESeverity::Error) << "Invalid Vertex Buffer Data Size\n";
            return;
        }

        VertexCount = Data.size();

        glBindBuffer(GL_ARRAY_BUFFER, VBO->Index);
        glBufferData(GL_ARRAY_BUFFER, Data.size()*VBO->Descriptor.Size, Data.data(), IsDynamic() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

        OGL::CheckErrorOpenGL();
    }
    else
    {
        LOG(ESeverity::Error) << "Using Invalid Vertex Buffer\n";
    }
}

void COGLVertexBuffer::SetData(const EVertexElement Type, const std::vector<Vector2>& Data)
{
    if (IsValid())
    {
        const SVBO* VBO = GetVBO(Type);
        if (!VBO)
        {
            LOG(ESeverity::Error) << "Vertex Buffer Do Not Support this Vertex Type\n";
            return;
        }
        if( Data.empty() )
        {
            LOG(ESeverity::Error) << "Invalid Vertex Buffer Data Size\n";
            return;
        }

        VertexCount = Data.size();

        glBindBuffer(GL_ARRAY_BUFFER, VBO->Index);
        glBufferData(GL_ARRAY_BUFFER, Data.size() * VBO->Descriptor.Size, Data.data(), IsDynamic() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

        OGL::CheckErrorOpenGL();
    }
    else
    {
        LOG(ESeverity::Error) << "Using Invalid Vertex Buffer\n";
    }
}

void COGLVertexBuffer::SetData(const EVertexElement Type, const std::vector<Color>& Data)
{
    if (IsValid())
    {
        const SVBO* VBO = GetVBO(Type);
        if (!VBO)
        {
            LOG(ESeverity::Error) << "Vertex Buffer Do Not Support this Vertex Type\n";
            return;
        }
        if( Data.empty() )
        {
            LOG(ESeverity::Error) << "Invalid Vertex Buffer Data Size\n";
            return;
        }

        VertexCount = Data.size();

        glBindBuffer(GL_ARRAY_BUFFER, VBO->Index);
        glBufferData(GL_ARRAY_BUFFER, Data.size() * VBO->Descriptor.Size, Data.data(), IsDynamic() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

        OGL::CheckErrorOpenGL();
    }
    else
    {
        LOG(ESeverity::Error) << "Using Invalid Vertex Buffer\n";
    }
}

void COGLVertexBuffer::SetIndices(const std::vector<unsigned int>& Data)
{
    if( IsValid() )
    {
        if( Data.empty() )
        {
            LOG(ESeverity::Error) << "Invalid Vertex Buffer Data Size\n";
            return;
        }

        IndicesCount = Data.size();

        if( !HasEBO )
        {
            glBindVertexArray(VAO);

            glGenBuffers(1, &EBO);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesCount * sizeof(unsigned int), nullptr, IsDynamic() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

            glBindVertexArray(0);

            HasEBO = true;
        }
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesCount*sizeof(unsigned int), Data.data(), IsDynamic() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

        OGL::CheckErrorOpenGL();
    }
    else
    {
        LOG(ESeverity::Error) << "Using Invalid Vertex Buffer\n";
    }
}
