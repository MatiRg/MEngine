#pragma once
#include "../Math/Rect2.hpp"
#include "../Math/Color.hpp"
#include "../Core/NonCopyable.hpp"
#include <string>
#include <memory>

class ISurface: public NonCopyableMovable
{
public:
    ISurface() = default;
    virtual ~ISurface() = default;

    virtual bool Create(const int, const int) = 0;
    virtual bool Load(const std::string&) = 0;
    virtual bool Save(const std::string&) = 0;

    virtual bool IsValid() const = 0;
    virtual bool SetColorKey(const Color&) = 0;

    virtual bool Fill(const Color&) = 0;
    virtual bool Fill(const Color&, const Rect2&) = 0;
    virtual bool Blit(ISurface*, const Rect2&) = 0;

    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;
    virtual int GetBPP() const = 0;
};

