#include "NullSound.hpp"

CNullSound::CNullSound()
{
}

CNullSound::~CNullSound()
{
}

bool CNullSound::Play()
{
    return true;
}

bool CNullSound::Pause(const bool)
{
    return true;
}

bool CNullSound::Stop()
{
    return true;
}

bool CNullSound::SetVolume(const float)
{
    return true;
}
