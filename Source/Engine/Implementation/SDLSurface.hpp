#pragma once
#include "../Graphics/Surface.hpp"

struct SDL_Surface;

class CSDLSurface: public ISurface
{
public:
    CSDLSurface();
    ~CSDLSurface();

    bool Create(SDL_Surface*);
    bool Create(const int, const int) override;
    bool Load(const std::string&) override;
    bool Save(const std::string&) override;

    bool IsValid() const override { return Surface; }
    bool SetColorKey(const Color&) override;

    bool Fill(const Color&) override;
    bool Fill(const Color&, const Rect2&) override;
    bool Blit(ISurface*, const Rect2&) override;

    int GetWidth() const override { return Width; }
    int GetHeight() const override { return Height; }
    int GetBPP() const override { return BPP; }

    SDL_Surface* GetSDLSurface() const { return Surface; }
private:
    void Destroy();
private:
    SDL_Surface* Surface = nullptr;
    int Width = 0;
    int Height = 0;
    int BPP = 0;
    Color ColorKey;
};

