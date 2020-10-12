#include "Shader.hpp"
#include "Graphics.hpp"

CShaderFactory::CShaderFactory(IGraphics* aGraphics):
    Graphics(aGraphics)
{
}

std::unique_ptr<IResource> CShaderFactory::CreateResource(const std::string& Name, CResources* Resources)
{
    return Graphics->CreateShader( Name, Resources );
}