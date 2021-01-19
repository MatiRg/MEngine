#include "OALUtils.hpp"
#include "../../Core/Log.hpp"
#include <AL/al.h>

namespace OAL
{
    bool CheckOALError()
    {
        ALenum Error = alGetError();
        if( Error != AL_NO_ERROR )
        {
            LOG( ESeverity::Error ) << "OpenAL: " << Error << "\n";
            return true;
        }
        return false;
    }
}