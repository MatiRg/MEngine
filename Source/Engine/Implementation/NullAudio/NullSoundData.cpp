#include "NullSoundData.hpp"
#include "../../Core/Utils.hpp"

CNullSoundData::CNullSoundData(const std::string& Name, CResources* Resources):
    ISoundData(Name, Resources)
{
}

CNullSoundData::~CNullSoundData()
{
}

bool CNullSoundData::Load()
{
    return true;
}
