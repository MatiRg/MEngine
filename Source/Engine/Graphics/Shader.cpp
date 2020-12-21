#include "Shader.hpp"
#include "Graphics.hpp"

bool IShader::HasUniform(const std::string& aName) const
{
    for (const auto& i : Uniforms)
    {
        if (i.Name == aName)
        {
            return true;
        }
    }
    return false;
}

//

CShaderManager::CShaderManager(IGraphics* aGraphics):
    Graphics(aGraphics)
{
}

std::unique_ptr<IResource> CShaderManager::MakeResource(const std::string& Name, const ResourceCreateMap&)
{
    return Graphics->CreateShader(Name);
}