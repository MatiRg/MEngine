#include "Texture2D.hpp"
#include "Graphics.hpp"

void ITexture2D::SetWrap(const ETextureWrap Wrap)
{
    SetWrapS(Wrap);
    SetWrapT(Wrap);
}

//

CTextureManager::CTextureManager(IGraphics* aGraphics):
    Graphics(aGraphics)
{
}

std::unique_ptr<IResource> CTextureManager::MakeResource(const std::string& Name, const ResourceCreateMap&)
{
    return Graphics->CreateTexture2D( Name );
}