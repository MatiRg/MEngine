#pragma once
#include "../Core/NonCopyable.hpp"
#include "../System//MemoryManager.hpp"
#include "Texture2D.hpp"

class IFrameBuffer: public NonCopyableMovable 
{
public:
    IFrameBuffer()
    {
        ADD_MEMORY_RECORD(this);
    }

    virtual ~IFrameBuffer() 
    {
        ERASE_MEMORY_RECORD(this);
    }

    virtual bool Create() = 0;

    virtual bool IsValid() const = 0;

    virtual ITexture2D* GetColorAttachment() const = 0;
    virtual ITexture2D* GetDepthAttachment() const = 0;

    virtual void Bind() = 0;
    virtual void UnBind() = 0;
};