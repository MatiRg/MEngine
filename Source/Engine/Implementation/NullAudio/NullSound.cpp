#include "NullSound.hpp"

CNullSound::CNullSound(ISoundData* aData):
	Data(aData)
{
}

CNullSound::~CNullSound()
{
}

void CNullSound::Play()
{
}

void CNullSound::Pause(const bool)
{
}

void CNullSound::Stop()
{
	Offset = 0;
}
