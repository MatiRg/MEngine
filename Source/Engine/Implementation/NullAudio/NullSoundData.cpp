#include "NullSoundData.hpp"
#include "../../Core/Utils.hpp"

CNullSoundData::CNullSoundData(const std::string& Name):
    ISoundData(Name)
{
}

CNullSoundData::~CNullSoundData()
{
}

bool CNullSoundData::Load(CResources*, const ResourceCreateMap&)
{
    Valid = true;
    return true;
}
