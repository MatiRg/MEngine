#include "Texture2D.hpp"
#include "Graphics.hpp"

CTexture2DFactory::CTexture2DFactory(IGraphics* aGraphics):
    Graphics(aGraphics)
{
}

std::unique_ptr<IResource> CTexture2DFactory::CreateResource(const std::string& Name, CResources* Resources)
{
    return Graphics->CreateTexture2D( Name, Resources );
}