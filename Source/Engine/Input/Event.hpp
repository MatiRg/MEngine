#pragma once
#include "../Math/Vector2.hpp"
#include "../Core/NonCopyable.hpp"
#include "EventTypes.hpp"
#include <memory>
#include <string>

struct SEventData
{
    SEventData() = default;
    virtual ~SEventData() = default;
};

struct SEventKeyboard: public SEventData
{
    SEventKeyboard(const EKey K, const EKeyMod M, const bool D, const bool P):
        Key( K ),
        Mod( M ),
        Down( D ),
        Pressed( P )
    {
    }

    EKey Key;
    EKeyMod Mod;
    bool Down;
    bool Pressed;
};

struct SEventMouseButton: public SEventData
{
    SEventMouseButton(const EMouseKey K, const bool D):
        Key( K ),
        Down( D )
    {
    }

    EMouseKey Key;
    bool Down;
};

struct SEventMouseMotion: public SEventData
{
    SEventMouseMotion(const Vector2& P, const Vector2& R):
        Position( P ),
        Relative( R )
    {
    }

    Vector2 Position;
    Vector2 Relative;
};

struct SEventMouseWheel: public SEventData
{
    SEventMouseWheel(const float X, const float Y):
        x( X ),
        y( Y )
    {
    }

    float x; // + right, - left
    float y; // + up , - down
};

struct SEventTextInput: public SEventData
{
    SEventTextInput(const std::string& D):
        Data( D )
    {
    }

    std::string Data;
};

struct SEventWindowSize: public SEventData
{
    SEventWindowSize(const int X, const int Y):
        x( X ),
        y( Y )
    {
    }

    int x;
    int y;
};

class CEvent: public NonCopyable
{
public:
    // Event is taking ownership of data
    CEvent(const EEventType T = EEventType::Null, std::unique_ptr<SEventData>&& D = {}):
        Type( T ),
        Data( std::move(D) )
    {
    }

    CEvent(CEvent&& Other):
        Type(std::move(Other.Type)),
        Data(std::move(Other.Data))
    {
    }

    ~CEvent() {}

    CEvent& operator=(CEvent&& Other)
    {
        Type = std::move(Other.Type);
        Data = std::move(Other.Data);
        return *this;
    }

    EEventType GetType() const { return Type; }
    bool HasData() const { return Data.get(); }
    template<class T>
    T* GetData() const { return static_cast<T*>(Data.get()); }
private:
    EEventType Type;
    std::unique_ptr<SEventData> Data;
};
