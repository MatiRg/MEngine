#pragma once
#include "../../Graphics/Texture2D.hpp"
#include "OGLUtils.hpp"

class COGLGraphics;

class COGLTexture2D: public ITexture2D
{
public:
    COGLTexture2D(const std::string&, CResources*, COGLGraphics*);
    ~COGLTexture2D();

    bool Load() override;
    bool CreateFromSurface(ISurface*) override;

    bool IsValid() const override { return Valid; }

    int GetWidth() const override { return Width; }
    int GetHeight() const override { return Height; }

    GLuint GetHandle() const { return Handle; }
private:
    bool Valid = false;
    COGLGraphics* Graphics = nullptr;
    int Width = 0;
    int Height = 0;
    GLuint Handle = 0u;
};
