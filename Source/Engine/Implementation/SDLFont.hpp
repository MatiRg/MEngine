#pragma once
#include "../Graphics/Font.hpp"

class IGraphics;

class CSDLFont: public IFont
{
public:
    CSDLFont(const std::string&, IGraphics*);
    ~CSDLFont();

    bool Load(CResources*, const ResourceCreateMap&) override;

    CFontCache* CreateFontCache(const int) override;
private:
    IGraphics* Graphics = nullptr;
    std::vector<char> Data;
};
