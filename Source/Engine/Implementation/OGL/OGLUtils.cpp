#include "OGLUtils.hpp"
#include "../../Core/Log.hpp"
#include <unordered_map>

namespace OGL
{
    namespace
    {
        const std::unordered_map<GLenum, std::string> ErrorDescriptions
        {
            { GL_NO_ERROR, "No Error" },
            { GL_INVALID_ENUM, "Invalid Enum" },
            { GL_INVALID_VALUE, "Invalid Value" },
            { GL_INVALID_OPERATION, "Invalid Operation" },
            { GL_OUT_OF_MEMORY, "Out of Memory" }
        };
    }

    bool CheckErrorOpenGL()
    {
        GLenum Error = glGetError();
        if( Error != GL_NO_ERROR )
        {
            if( ErrorDescriptions.count(Error) )
            {
                LOG( ESeverity::Error ) << "OpenGL: " << ErrorDescriptions.at(Error) << "\n";
            }
            else
            {
                LOG( ESeverity::Error ) << "OpenGL: Unknown Error Code - " << Error << "\n";
            }
            return true;
        }
        return false;
    }
}