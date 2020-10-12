#pragma once
#include "../Core/NonCopyable.hpp"

struct ImDrawData;

class IImGUIRenderer: public NonCopyableMovable
{
public:
    IImGUIRenderer() = default;
    virtual ~IImGUIRenderer() = default;

    virtual bool Init() = 0;
    virtual void Update(const float) = 0;
    virtual void Draw(ImDrawData*) = 0;
    virtual void Destroy() = 0;
};