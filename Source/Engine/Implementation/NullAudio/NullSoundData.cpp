#include "NullSoundData.hpp"
#include "../../Core/Utils.hpp"

CNullSoundData::CNullSoundData(const std::string& Name):
    ISoundData(Name)
{
}

CNullSoundData::~CNullSoundData()
{
}

bool CNullSoundData::Load(CResources*, const ResourceCreateMap& VarMap)
{
    Stream = !VarMap.count(RESOURCES_VAR_IS_STREAM) ? false : std::any_cast<bool>(VarMap.at(RESOURCES_VAR_IS_STREAM));

    Valid = true;
    return true;
}
