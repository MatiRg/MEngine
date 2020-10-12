#pragma once
#include "../Graphics/Font.hpp"

class IGraphics;

class CSDLFont: public IFont
{
public:
    CSDLFont(const std::string&, CResources*, IGraphics*);
    ~CSDLFont();

    bool Load() override;
    bool IsValid() const override { return Valid; }

    CFontCache* CreateFontCache(const int) override;
private:
    IGraphics* Graphics = nullptr;
    std::vector<char> Data;
    bool Valid = false;
};
