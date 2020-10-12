#include "AudioListener2D.hpp"
#include "../Entity2D.hpp"
#include "../../Core/XML.hpp"
#include "../../Audio/Audio.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Math/Functions.hpp"

CAudioListener2D::CAudioListener2D(CEngine* aEngine): IComponent(aEngine) 
{
    Audio = Engine->GetAudio();
}

bool CAudioListener2D::OnLoad(CXMLElement* Root)
{
	Muted = XML::LoadBool( Root, "Muted", false );
    SetMuted(Muted);
	Volume = XML::LoadFloat( Root, "Volume", 1.0f);
    SetVolume(Volume);
    return true;
}

bool CAudioListener2D::OnSave(CXMLElement* Root)
{
    XML::SaveBool( Root, "Muted", Muted);
    XML::SaveFloat( Root, "Volume", Volume);
    return true;
}

void CAudioListener2D::OnEnabled()
{
    Audio->SetMuted(Muted);
}

void CAudioListener2D::OnDisabled()
{
    Audio->SetMuted(true);
}

void CAudioListener2D::OnCreate()
{
    OrginalMute = Audio->IsMuted();
    Audio->SetMuted(Muted);

    OrginalVolume = Audio->GetVolume();
    Audio->SetVolume(Volume);
}

void CAudioListener2D::OnDestroy()
{
    Audio->SetMuted(OrginalMute);
    Audio->SetVolume(OrginalVolume);
}

void CAudioListener2D::OnBeginFrame()
{
    auto Transform = GetOwner()->GetTransform();
    Audio->SetListenerPosition( Vector3(Transform.GetWorldPosition(), 0.0f) );
}

void CAudioListener2D::SetMuted(const bool Value)
{
    Muted = Value;
    Audio->SetMuted(Muted);
}

void CAudioListener2D::SetVolume(const float aVolume)
{
    Volume = Math::Clamp( aVolume, 0.0f, 1.0f );
    Audio->SetVolume(Volume);
}