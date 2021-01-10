#pragma once
#include "../Resources/ResourceManager.hpp"
#include "../Math/Vector2.hpp"
#include "../Math/Vector3.hpp"
#include "../Math/Quaternion.hpp"
#include "Texture2D.hpp"
#include <string>
#include <map>
#include <vector>
#include <memory>

class IGraphics;
class IVertexBuffer;
class CModel;
class CMeshNode;

struct aiNode;
struct aiScene;
struct aiMesh;

class CMesh: public NonCopyableMovable
{
public:
    CMesh(IGraphics*, CMeshNode*);
    ~CMesh();

    CMeshNode* GetMeshNode() const { return Node; }

    // Position, UVs, Normals, Indices
    void Create(const std::vector<Vector3>&, const std::vector<Vector2>&, const std::vector<Vector3>&, const std::vector<unsigned int>&);

    void SetName(const std::string& aName) { Name = aName; }
    const std::string& GetName() const { return Name; }

    IVertexBuffer* GetBuffer() const { return Buffer.get(); }
private:
    IGraphics* Graphics = nullptr;
    CMeshNode* Node = nullptr;
    std::string Name;
    std::unique_ptr<IVertexBuffer> Buffer;
};

using MeshArray = std::vector<CMesh*>;

class CMeshNode : public NonCopyableMovable
{
public:
    CMeshNode(CMeshNode* aParent = nullptr);
    ~CMeshNode();

    CMeshNode* GetParent() const { return Parent; }

    void SetName(const std::string& aName) { Name = aName; }
    const std::string& GetName() const { return Name; }

    const std::vector<CMeshNode*>& GetChildren() const { return Children; }

    void SetPosition(const Vector3& aPosition) { Position = aPosition; }
    const Vector3& GetPosition() const { return Position; }

    void SetScale(const Vector3& aScale) { Scale = aScale; }
    const Vector3& GetScale() const { return Scale; }

    void SetRotation(const Quaternion& aRotation) { Rotation = aRotation; }
    const Quaternion& GetRotation() const { return Rotation; }

    void AddMesh(CMesh* aMesh) { if (aMesh) Meshes.push_back(aMesh); }
    CMesh* GetMesh(const std::string&) const;
    const MeshArray& GetMeshes() const { return Meshes; }
private:
    void AddChild(CMeshNode* aNode) { if (aNode) Children.push_back(aNode); }
private:
    std::string Name;
    CMeshNode* Parent = nullptr;
    std::vector<CMeshNode*> Children;
    Vector3 Position;
    Vector3 Scale;
    Quaternion Rotation;
    MeshArray Meshes;
};

class CModel: public IResource
{
public:
    CModel(const std::string&, IGraphics*);
    virtual ~CModel();

    RESOURCE(CModel)

    bool Load(CResources*, const ResourceCreateMap&) override;
    
    CMeshNode* GetRoot() { return &Root; }

    CMesh* GetMesh(const std::string&) const;
    const MeshArray& GetMeshes() const { return Meshes; }
private:
    void ProcessNode(aiNode*, const aiScene*, CMeshNode*);
    void ProcessMesh(aiMesh*, const aiScene*, CMeshNode*);
private:
    CMeshNode Root = {};
    MeshArray Meshes;
    IGraphics* Graphics = nullptr;
};

//

class CModelManager : public TResourceManager<CModel>
{
public:
    CModelManager(IGraphics*);

    RESOURCE_MANAGER(CModelManager)
protected:
    std::unique_ptr<IResource> MakeResource(const std::string&, const ResourceCreateMap&) override;
private:
    IGraphics* Graphics = nullptr;
};