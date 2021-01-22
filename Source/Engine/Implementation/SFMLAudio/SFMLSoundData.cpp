#include "SFMLSoundData.hpp"
#include "../../Core/Log.hpp"
#include "../../Core/BinaryFile.hpp"
#include "../../Core/Utils.hpp"

CSFMLSoundData::CSFMLSoundData(const std::string& Name):
    ISoundData(Name)
{
}

CSFMLSoundData::~CSFMLSoundData()
{
}

bool CSFMLSoundData::Load(CResources*, const ResourceCreateMap&)
{
    if( !Buffer.loadFromFile( GetPath() ) )
    {
        LOG( ESeverity::Error ) << "Unable to Load Sound File: " << GetName() << "\n";
        return false;
    }
    Valid = true;
    return Valid;
}

int CSFMLSoundData::GetDuration() const
{
    if( !IsValid() )
    {
        return 0;
    }
    return Buffer.getDuration().asMilliseconds();
}
