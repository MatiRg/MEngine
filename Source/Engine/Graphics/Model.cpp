#include "Model.hpp"
#include "Graphics.hpp"
#include "Material.hpp"
#include "../Resources/Resources.hpp"
#include "../System/MemoryManager.hpp"
#include "../Core/Log.hpp"
#include "../Core/Utils.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

CMeshNode::CMeshNode(CMeshNode* aParent):
    Parent(aParent)
{
    if (Parent)
    {
        Parent->AddChild(this);
    }
    ADD_MEMORY_RECORD(this);
}

CMeshNode::~CMeshNode()
{
    for (const auto& i : Children)
    {
        delete i;
    }
    ERASE_MEMORY_RECORD(this);
}

CMesh* CMeshNode::GetMesh(const std::string& aName) const
{
    for (const auto& i : Meshes)
    {
        if (i->GetName() == aName)
        {
            return i;
        }
    }
    return nullptr;
}

//

CMesh::CMesh()
{
    ADD_MEMORY_RECORD(this);
}

CMesh::~CMesh()
{
    Buffer.reset();
    ERASE_MEMORY_RECORD(this);
}

//

CModel::CModel(const std::string& Name, IGraphics* aGraphics):
    IResource( Name ),
	Graphics(aGraphics)
{
}

CModel::~CModel()
{
    for (const auto& i : Meshes)
    {
        delete i;
    }
}

bool CModel::Load(CResources* Resources, const ResourceCreateMap&)
{
    Assimp::Importer Importer;

    unsigned int Flags = aiProcessPreset_TargetRealtime_Fast | aiProcess_MakeLeftHanded | aiProcess_OptimizeGraph;
    //Flags |= aiProcess_FlipUVs;

    const aiScene* Scene = Importer.ReadFile(GetPath(), Flags);
    if( !Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode )
    {
        LOG(ESeverity::Error) << "Mesh - " << GetPath() << ": " << Importer.GetErrorString() << "\n";
        return false;
    }

    Material = Resources->CreateResource<CMaterial>(Utils::GetNameFromExt(GetName())+".mat" );
    if (!Material)
    {
        LOG(ESeverity::Warning) << "Switching to Default Material\n";
        Material = Resources->CreateResource<CMaterial>("Default.mat");
        if (!Material)
        {
            LOG(ESeverity::Error) << "No Default Material\n";
            return false;
        }
    }

    ProcessNode(Scene->mRootNode, Scene, GetRoot() );

    Valid = true;
    return true;
}

CMesh* CModel::GetMesh(const std::string& aName) const
{
    for (const auto& i : Meshes)
    {
        if (i->GetName() == aName)
        {
            return i;
        }
    }
    return nullptr;
}

void CModel::ProcessNode(aiNode* Node, const aiScene* Scene, CMeshNode* MeshNode)
{
    if (!Node || !Scene)
    {
        return;
    }

    aiVector3D Scaling;
    aiVector3D Position;
    aiQuaternion Rotation;
    Node->mTransformation.Decompose(Scaling, Rotation, Position);
    MeshNode->SetPosition( {Position.x, Position.y, Position.z} );
    MeshNode->SetScale({ Scaling.x, Scaling.y, Scaling.z });
    MeshNode->SetRotation({ Rotation.x, Rotation.y, Rotation.z, Rotation.w });
    MeshNode->SetName(Node->mName.C_Str());

    for (unsigned int i = 0; i < Node->mNumMeshes; i++)
    {
        aiMesh* AssimpMesh = Scene->mMeshes[Node->mMeshes[i]];
        ProcessMesh(AssimpMesh, Scene, MeshNode);
    }

    for (unsigned int i = 0; i < Node->mNumChildren; i++)
    {
        CMeshNode* Next = new CMeshNode(MeshNode);
        ProcessNode(Node->mChildren[i], Scene, Next);
    }
}


void CModel::ProcessMesh(aiMesh* AssimpMesh, const aiScene* Scene, CMeshNode* MeshNode)
{
    std::vector<Vector3> Position;
    std::vector<Vector2> UVs;
    std::vector<Vector3> Normals;
    std::vector<unsigned int> Indices;
    std::string Name;

    for (unsigned int i = 0u; i < AssimpMesh->mNumVertices; ++i)
    {
        Position.push_back({ AssimpMesh->mVertices[i].x, AssimpMesh->mVertices[i].y, AssimpMesh->mVertices[i].z });
        Normals.push_back({ AssimpMesh->mNormals[i].x, AssimpMesh->mNormals[i].y, AssimpMesh->mNormals[i].z });

        if (AssimpMesh->mTextureCoords[0])
        {
            UVs.push_back({ AssimpMesh->mTextureCoords[0][i].x, AssimpMesh->mTextureCoords[0][i].y });
        }
        else
        {
            UVs.push_back({ 0.0f, 0.0f });
        }
    }
    for (unsigned int i = 0u; i < AssimpMesh->mNumFaces; ++i)
    {
        aiFace face = AssimpMesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            Indices.push_back(face.mIndices[j]);
        }
    }
    Name = AssimpMesh->mName.C_Str();

    if (!Position.empty())
    {     
        std::unique_ptr<IVertexBuffer> Buffer = Graphics->CreateVertexBuffer({ EVertexElement::Position, EVertexElement::TexCoord0, EVertexElement::Normal }, false);
        Buffer->SetData(EVertexElement::Position, Position);
        Buffer->SetData(EVertexElement::TexCoord0, UVs);
        Buffer->SetData(EVertexElement::Normal, Normals);
        Buffer->SetIndices(Indices);

        CMesh* Mesh = new CMesh();
        Mesh->SetName(Name);
        Mesh->SetMeshNode(MeshNode);
        Mesh->SetMaterial(Material);
        Mesh->SetVertexBuffer( std::move(Buffer) );

        Meshes.push_back(Mesh);
        MeshNode->AddMesh(Mesh);
    }
}

//

CModelManager::CModelManager(IGraphics* aGraphics):
    Graphics(aGraphics)
{
}

std::unique_ptr<IResource> CModelManager::MakeResource(const std::string& Name, const ResourceCreateMap&)
{
    return std::make_unique<CModel>(Name, Graphics);
}