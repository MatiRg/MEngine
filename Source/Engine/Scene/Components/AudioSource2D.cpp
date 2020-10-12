#include "AudioSource2D.hpp"
#include "../Entity2D.hpp"
#include "../../Core/XML.hpp"
#include "../../Audio/Audio.hpp"
#include "../../Audio/SoundData.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Math/Functions.hpp"
#include "../../Resources/Resources.hpp"

CAudioSource2D::CAudioSource2D(CEngine* aEngine): IComponent(aEngine)
{
}

bool CAudioSource2D::OnLoad(CXMLElement* Root)
{
    Muted = XML::LoadBool( Root, "Muted", false );
    SetMuted(Muted);
    Volume = XML::LoadFloat( Root, "Volume", 1.0f );
    SetVolume(Volume);
	Looped = XML::LoadBool( Root, "Looped", false );
    SetLooped(Looped);

    if( XML::LoadBool(Root, "HasData", false) )
    {
        std::string DataName = XML::LoadString( Root, "DataName", "");
        Data = Engine->GetResources()->CreateResource<ISoundData>(DataName);
        if( !Data )
        {
            LOG(ESeverity::Error) << "Invalid Sound Data: " << DataName << "\n";
            return false;
        }
    }
    SetData(Data);

    if( Sound )
    {
        int Offset = XML::LoadInt( Root, "Offset", 0);
        Sound->SetOffset(Offset);

        ESoundState State = XML::LoadEnum( Root, "State", ESoundState::Playing );
        if( State < ESoundState::Invalid || State > ESoundState::Playing )
        {
            LOG(ESeverity::Error) << "ESoundState Enum Invalid Value\n";
            return false;
        }

		OldState = XML::LoadEnum( Root, "OldState", ESoundState::Playing);
        if( OldState < ESoundState::Invalid || OldState > ESoundState::Playing )
        {
            LOG(ESeverity::Error) << "ESoundState Enum Invalid Value\n";
            return false;
        }

        if( State == ESoundState::Playing )
        {
            Play();
        }
        else if( State == ESoundState::Paused )
        {
            Pause(true);
        }
    }

    return true;
}

bool CAudioSource2D::OnSave(CXMLElement* Root)
{
    XML::SaveBool( Root, "Muted", Muted);
    XML::SaveFloat( Root, "Volume", Volume);
    XML::SaveBool( Root, "Looped", Looped);

    if( Data )
    {
        XML::SaveBool( Root, "HasData", true);
        XML::SaveString( Root, "DataName", Data->GetName());
    }
    else
    {
        XML::SaveBool( Root, "HasData", false );
    }

    if( Sound )
    {
        XML::SaveInt( Root, "Offset", Sound->GetOffset() );
        XML::SaveEnum( Root, "State", Sound->GetState());
        XML::SaveEnum( Root, "OldState", OldState);
    }

    return true;
}

void CAudioSource2D::OnEnabled()
{
    if( Sound )
    {
        if( OldState == ESoundState::Playing ) 
        {
            Play();
        }
    }
}

void CAudioSource2D::OnDisabled()
{
    if( Sound )
    {
        OldState = Sound->GetState();
        if( OldState == ESoundState::Playing ) 
        {
            Pause(true);
        }
    }
}

void CAudioSource2D::OnCreate()
{
    Sound = Engine->GetAudio()->CreateSound();
}

void CAudioSource2D::OnDestroy()
{
    Stop();
    Engine->GetAudio()->DestroySound(Sound);
}

void CAudioSource2D::OnBeginFrame()
{
    if( Sound )
    {
        auto Transform = GetOwner()->GetTransform();
        Sound->SetPosition( Vector3(Transform.GetWorldPosition(), 0.0f) );
    }
}

void CAudioSource2D::SetMuted(const bool Value)
{
    Muted = Value;
    if( Sound )
    {
        Sound->SetMuted(Muted);
    }
}

void CAudioSource2D::SetVolume(const float aVolume)
{
    Volume = Math::Clamp( aVolume, 0.0f, 1.0f );
    if( Sound )
    {
        Sound->SetVolume(Volume);
    }
}

void CAudioSource2D::SetLooped(const bool Value)
{
    Looped = Value;
    if( Sound )
    {
        Sound->SetLooped(Looped);
    }
}

void CAudioSource2D::Play()
{
    if( Sound )
    {
        Sound->Play();
    }
}

void CAudioSource2D::Pause(const bool State)
{
    if( Sound )
    {
        Sound->Pause(State);
    }
}

void CAudioSource2D::Stop()
{
    if( Sound )
    {
        Sound->Stop();
    }
}

void CAudioSource2D::SetData(ISoundData* aData)
{
    Data = aData;
    if( Sound )
    {
        Stop();
        Sound->SetSoundData( Data );
    }
}
