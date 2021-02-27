#include "Transform.hpp"
#include "../Core/XML.hpp"
#include "../System/MemoryManager.hpp"
#include <algorithm> 

CTransform::CTransform()
{
    ADD_MEMORY_RECORD(this);
}

CTransform::~CTransform()
{
    if (Parent)
    {
        Parent->RemoveChild(this);
    }
    //
    ERASE_MEMORY_RECORD(this);
}

bool CTransform::Load(CXMLElement* Root)
{
	Position = XML::LoadVector3(Root, "Position", Vector3::ZERO );
	Scale = XML::LoadVector3(Root, "Scale", Vector3::ONE );
    Rotation = XML::LoadQuaternion(Root, "Rotation", Quaternion::IDENTITY);
    Dirty = XML::LoadBool(Root, "Dirty", true);
    WorldRotation = XML::LoadQuaternion(Root, "WorldRotation", Quaternion::IDENTITY);
    InvWorldRotation = XML::LoadQuaternion(Root, "InvWorldRotation", Quaternion::IDENTITY);
    Matrix = XML::LoadMatrix4(Root, "Matrix", Matrix4::IDENTITY);
    InvMatrix = XML::LoadMatrix4(Root, "InvMatrix", Matrix4::IDENTITY);
    WorldMatrix = XML::LoadMatrix4(Root, "WorldMatrix", Matrix4::IDENTITY);
    InvWorldMatrix = XML::LoadMatrix4(Root, "InvWorldMatrix", Matrix4::IDENTITY);
    return true;
}

bool CTransform::Save(CXMLElement* Root)
{
	XML::SaveVector3(Root, "Position", Position);
	XML::SaveVector3(Root, "Scale", Scale);
	XML::SaveQuaternion(Root, "Rotation", Rotation);
    XML::SaveBool(Root, "Dirty", Dirty);
    XML::SaveQuaternion(Root, "WorldRotation", WorldRotation);
    XML::SaveQuaternion(Root, "InvWorldRotation", InvWorldRotation);
    XML::SaveMatrix4(Root, "Matrix", Matrix);
    XML::SaveMatrix4(Root, "InvMatrix", InvMatrix);
    XML::SaveMatrix4(Root, "WorldMatrix", WorldMatrix);
    XML::SaveMatrix4(Root, "InvWorldMatrix", InvWorldMatrix);
    return true;
}

void CTransform::SetParent(CTransform* aParent)
{
    // Old Parent
    if (Parent)
    {
        Parent->RemoveChild(this);
    }
    //
    Parent = aParent;
    if (Parent)
    {
        Parent->AddChild(this);
    }
}

void CTransform::AddChild(CTransform* Other)
{
    if (Other)
    {
        Children.push_back(Other);
    }
}

void CTransform::RemoveChild(CTransform* Other)
{
    if (Other)
    {
        auto Iterator = std::find_if(Children.begin(), Children.end(), [&](CTransform* Transform)
        {
            return Transform == Other;
        });
        if (Iterator != Children.end())
        {
            Children.erase(Iterator);
        }
    }
}

void CTransform::SetPosition(const Vector3& aPosition, const bool Silent)
{
    Position = aPosition;
    MarkDirty();
    if (!Silent)
    {    
        for (const auto& i : PositionCallback)
        {
            i.second(Position);
        }
    }
}

void CTransform::SetScale(const Vector3& aScale, const bool Silent)
{
    Scale = aScale;
    MarkDirty();
    if (!Silent)
    {    
        for (const auto& i : ScaleCallback)
        {
            i.second(Scale);
        }
    }
}

void CTransform::SetRotation(const Quaternion& aRotation, const bool Silent)
{
    Rotation = aRotation;
    MarkDirty();
    if (!Silent)
    {
        for (const auto& i : RotationCallback)
        {
            i.second(Rotation);
        }
    }
}

void CTransform::SetWorldPosition(const Vector3& aPosition, const bool Silent)
{
    if (HasParent())
    {
        Vector3 Tmp = Parent->GetInvWorldMatrix() * aPosition;
        SetPosition(Tmp, Silent);
    }
    else
    {
        SetPosition(aPosition, Silent);
    }
}

Vector3 CTransform::GetWorldPosition() const
{
    return GetWorldMatrix().GetTranslation();
}

void CTransform::SetWorldScale(const Vector3& aScale, const bool Silent)
{
    if (HasParent())
    {
        Vector3 Tmp = aScale / Parent->GetWorldScale();
        SetScale(Tmp, Silent);
    }
    else
    {
        SetScale(aScale, Silent);
    }
}

Vector3 CTransform::GetWorldScale() const
{
    return GetWorldMatrix().GetScale();
}

void CTransform::SetWorldRotation(const Quaternion& aRotation, const bool Silent)
{
    if (HasParent())
    {
        Quaternion Tmp = Parent->GetInvWorldRotation() * aRotation;
        SetRotation(Tmp, Silent);
    }
    else
    {
        SetRotation(aRotation, Silent);
    }
}

const Quaternion& CTransform::GetWorldRotation() const
{
    if (IsDirty())
    {
        RecalculateMatrix();
    }
    return WorldRotation;
}

const Quaternion& CTransform::GetInvWorldRotation() const
{
    if (IsDirty())
    {
        RecalculateMatrix();
    }
    return InvWorldRotation;
}

void CTransform::SetRight(const Vector3& NewRight)
{
    SetWorldRotation(Quaternion(Vector3::RIGHT, NewRight));
}

Vector3 CTransform::GetRight() const
{
    return GetWorldRotation() * Vector3::RIGHT;
}

void CTransform::SetUp(const Vector3& NewUp)
{
    SetWorldRotation(Quaternion(Vector3::UP, NewUp));
}

Vector3 CTransform::GetUp() const
{
    return GetWorldRotation() * Vector3::UP;
}

void CTransform::SetForward(const Vector3& NewForward)
{
    SetWorldRotation( Math::LookRotation(NewForward, Vector3::UP) );
}

Vector3 CTransform::GetForward() const
{
    return GetWorldRotation() * Vector3::FORWARD;
}

void CTransform::SetEulerAngles(const Vector3& Eulers)
{
    SetRotation(Quaternion(Eulers));
}

Vector3 CTransform::GetEulerAngles() const
{
    return GetRotation().ToEulerAngles();
}

void CTransform::SetWorldEulerAngles(const Vector3& Eulers)
{
    SetWorldRotation(Quaternion(Eulers));
}

Vector3 CTransform::GetWorldEulerAngles() const
{
    return GetWorldRotation().ToEulerAngles();
}

void CTransform::Translate(const Vector3& Delta)
{
    SetPosition( Position + Rotation*Delta );
}

void CTransform::Rotate(const Quaternion& Delta)
{
    SetRotation( Rotation*Delta );
}

const Matrix4& CTransform::GetMatrix() const
{
    if (IsDirty())
    {
        RecalculateMatrix();
    }
    return Matrix;
}

const Matrix4& CTransform::GetWorldMatrix() const
{
    if (IsDirty())
    {
        RecalculateMatrix();
    }
    return WorldMatrix;
}

const Matrix4& CTransform::GetInvMatrix() const
{
    if (IsDirty())
    {
        RecalculateMatrix();
    }
    return InvMatrix;
}

const Matrix4& CTransform::GetInvWorldMatrix() const
{
    if (IsDirty())
    {
        RecalculateMatrix();
    }
    return InvWorldMatrix;
}

void CTransform::AddPositionCallback(void* Key, const PositionChanged& Callback)
{
    PositionCallback.emplace_back(Key, Callback);
}

void CTransform::AddScaleCallback(void* Key, const ScaleChanged& Callback)
{
    ScaleCallback.emplace_back(Key, Callback);
}

void CTransform::AddRotationCallback(void* Key, const RotationChanged& Callback)
{
    RotationCallback.emplace_back( Key, Callback );
}

void CTransform::AddChangedCallback(void* Key, const TransformChanged& Callback)
{
    TransformChangedCallback.emplace_back(Key, Callback);
}

void CTransform::RemovePositionCallback(void* Key)
{
    PositionCallback.erase(std::remove_if(PositionCallback.begin(), PositionCallback.end(), [&](const std::pair<void*, PositionChanged>& Value)
    {
        return Value.first == Key;
    }), PositionCallback.end());
}

void CTransform::RemoveScaleCallback(void* Key)
{
    ScaleCallback.erase(std::remove_if(ScaleCallback.begin(), ScaleCallback.end(), [&](const std::pair<void*, ScaleChanged>& Value)
    {
        return Value.first == Key;
    }), ScaleCallback.end());
}

void CTransform::RemoveRotationCallback(void* Key)
{
    RotationCallback.erase(std::remove_if(RotationCallback.begin(), RotationCallback.end(), [&](const std::pair<void*, RotationChanged>& Value)
    {
        return Value.first == Key;
    }), RotationCallback.end());
}

void CTransform::RemoveChangedCallback(void* Key)
{
    TransformChangedCallback.erase(std::remove_if(TransformChangedCallback.begin(), TransformChangedCallback.end(), [&](const std::pair<void*, TransformChanged>& Value)
    {
        return Value.first == Key;
    }), TransformChangedCallback.end());
}

void CTransform::MarkDirty()
{
    Dirty = true;
    for (const auto& i : Children)
    {
        i->MarkDirty();
    }
    //
    for (const auto& i : TransformChangedCallback)
    {
        i.second(this);
    }
}

void CTransform::RecalculateMatrix() const
{
    WorldRotation = GetRotation();
    if (HasParent())
    {
        WorldRotation = Parent->GetWorldRotation() * WorldRotation;
    }
    InvWorldRotation = WorldRotation.Inverse();
    //
    Matrix = Math::Transform(Position, Rotation, Scale);
    InvMatrix = Matrix.Inverse();
    //
    WorldMatrix = Matrix;
    if (HasParent())
    {
        WorldMatrix = Parent->GetWorldMatrix() * WorldMatrix;
    }
    InvWorldMatrix = WorldMatrix.Inverse();
    //
    Dirty = false;
}
