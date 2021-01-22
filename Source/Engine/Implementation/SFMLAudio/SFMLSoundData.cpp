#include "SFMLSoundData.hpp"
#include "../../Core/Log.hpp"
#include "../../Core/BinaryFile.hpp"
#include "../../Core/Utils.hpp"
#include "../../System/System.hpp"
#include "../../Resources/Resources.hpp"

CSFMLSoundData::CSFMLSoundData(const std::string& Name):
    ISoundData(Name)
{
}

CSFMLSoundData::~CSFMLSoundData()
{
}

bool CSFMLSoundData::Load(CResources* Resources, const ResourceCreateMap& VarMap)
{
    Stream = !VarMap.count(RESOURCES_VAR_IS_STREAM) ? false : std::any_cast<bool>(VarMap.at(RESOURCES_VAR_IS_STREAM));

    if (Stream)
    {
        if ( !Resources->GetSystem()->FileExist(GetPath()) )
        {
            LOG(ESeverity::Error) << "Unable to Open Sound File: " << GetName() << "\n";
            return false;
        }
    }
    else
    {
        Buffer = std::make_unique<sf::SoundBuffer>();
        if (!Buffer->loadFromFile(GetPath()))
        {
            LOG(ESeverity::Error) << "Unable to Load Sound File: " << GetName() << "\n";
            return false;
        }
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
    return Buffer->getDuration().asMilliseconds();
}
