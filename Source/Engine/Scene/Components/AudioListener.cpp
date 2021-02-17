#include "AudioListener.hpp"
#include "../Entity.hpp"
#include "../../Core/XML.hpp"
#include "../../Audio/Audio.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Math/Functions.hpp"

CAudioListener::CAudioListener(CEngine* aEngine): IComponent(aEngine)
{
    Audio = Engine->GetAudio();
}

bool CAudioListener::OnLoad(CXMLElement* Root)
{
	Muted = XML::LoadBool( Root, "Muted", false );
    SetMuted(Muted);
	Volume = XML::LoadFloat( Root, "Volume", 1.0f);
    SetVolume(Volume);
    return true;
}

bool CAudioListener::OnSave(CXMLElement* Root)
{
    XML::SaveBool( Root, "Muted", Muted);
    XML::SaveFloat( Root, "Volume", Volume);
    return true;
}

void CAudioListener::OnEnabled()
{
    Audio->SetMuted(Muted);
}

void CAudioListener::OnDisabled()
{
    Audio->SetMuted(true);
}

void CAudioListener::OnCreate()
{
    OrginalMute = Audio->IsMuted();
    Audio->SetMuted(Muted);

    OrginalVolume = Audio->GetVolume();
    Audio->SetVolume(Volume);
}

void CAudioListener::OnDestroy()
{
    Audio->SetMuted(OrginalMute);
    Audio->SetVolume(OrginalVolume);
}

void CAudioListener::OnLateUpdate(const float)
{
    const auto& Transform = GetOwner()->GetTransform();

    Audio->SetListenerDirection( Transform.GetForward() );
    Audio->SetListenerPosition( Transform.GetWorldPosition() );
}

void CAudioListener::SetMuted(const bool Value)
{
    Muted = Value;
    Audio->SetMuted(Muted);
}

void CAudioListener::SetVolume(const float aVolume)
{
    Volume = Math::Clamp( aVolume, 0.0f, 1.0f );
    Audio->SetVolume(Volume);
}