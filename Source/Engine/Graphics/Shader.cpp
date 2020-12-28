#include "Shader.hpp"
#include "Graphics.hpp"
#include <algorithm>

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

CShaderManager::~CShaderManager()
{
    for (const auto& i : Shaders)
    {
        delete i;
    }
}

std::unique_ptr<IResource> CShaderManager::MakeResource(const std::string& Name, const ResourceCreateMap&)
{
    return Graphics->CreateShader(Name);
}

IResource* CShaderManager::GetResource(const std::string& Name, const ResourceCreateMap& Maps)
{
    StringVec Defines = !Maps.count(RESOURCES_VAR_DEFINES) ? StringVec() : std::any_cast<StringVec>(Maps.at(RESOURCES_VAR_DEFINES));
    std::sort(Defines.begin(), Defines.end());

    for (const auto& i : Shaders)
    {
        if (i->Shader->GetName() == Name && i->Defines == Defines)
        {
            return i->Shader.get();
        }
    }
    return nullptr;
}

IResource* CShaderManager::AddResource(std::unique_ptr<IResource>&& Ptr, const ResourceCreateMap& Maps)
{
    StringVec Defines = !Maps.count(RESOURCES_VAR_DEFINES) ? StringVec() : std::any_cast<StringVec>(Maps.at(RESOURCES_VAR_DEFINES));
    std::sort(Defines.begin(), Defines.end());
    //
    IResource* Tmp = Ptr.get();
    Shaders.push_back( new SShaderVariant(Defines, std::move(Ptr) ) );
    return Tmp;
}