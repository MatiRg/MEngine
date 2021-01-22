#include "SFMLAudio.hpp"
#include "SFMLSoundData.hpp"
#include "SFMLSound.hpp"
#include "../../Core/Log.hpp"
#include "../../Math/Functions.hpp"
#include "../../Core/Utils.hpp"

CSFMLAudio::CSFMLAudio()
{
}

CSFMLAudio::~CSFMLAudio()
{
}

bool CSFMLAudio::Init(const SEngineParams&)
{
    sf::Listener::setGlobalVolume( Volume * 100.0f);

    Vector3 Up = Vector3::UP;

    sf::Listener::setDirection( sf::Vector3f(Forward.x, Forward.y, Forward.z) );
    sf::Listener::setUpVector( sf::Vector3f(Up.x, Up.y, Up.z) );
    sf::Listener::setPosition( sf::Vector3f(Position.x, Position.y, Position.z) );

    LOG( ESeverity::Info ) << "Set Audio Device: SFML\n";
    LOG( ESeverity::Info ) << "Audio - Init\n";
    return true;
}

void CSFMLAudio::Exit()
{
    LOG( ESeverity::Info ) << "Audio - Exit\n";
}

std::unique_ptr<ISoundData> CSFMLAudio::CreateSoundData(const std::string& Name)
{
    return std::make_unique<CSFMLSoundData>( Name );
}

std::unique_ptr<ISound> CSFMLAudio::CreateSound(ISoundData* Data)
{
    auto Sound = std::make_unique<CSFMLSound>( dynamic_cast<CSFMLSoundData*>(Data) );
    if( !Sound->CreateSource() )
    {
        return nullptr;
    }
    return Sound;
}

void CSFMLAudio::SetVolume(const float aVolume)
{
    Volume = Math::Clamp( aVolume, 0.0f, 1.0f );
    sf::Listener::setGlobalVolume(Volume * 100.0f);
}

void CSFMLAudio::SetMuted(const bool Value)
{ 
    Muted = Value; 
    sf::Listener::setGlobalVolume(Muted ? 0.0f : (Volume*100.0f) );
}

void CSFMLAudio::SetListenerPosition(const Vector3& aPosition)
{
    Position = aPosition;
    sf::Listener::setPosition(sf::Vector3f(Position.x, Position.y, Position.z));
}

void CSFMLAudio::SetListenerDirection(const Vector3& aForward)
{
    Forward = aForward;
    sf::Listener::setDirection(sf::Vector3f(Forward.x, Forward.y, Forward.z));
}

void CSFMLAudio::SetListenerVelocity(const Vector3& aVelocity)
{
    // Not Supported By SFML
    Velocity = aVelocity;
}