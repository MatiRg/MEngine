#include "Transform.hpp"
#include "../Core/XML.hpp"
#include <algorithm> 

CTransform::CTransform()
{
}

CTransform::CTransform(const CTransform& Other):
    Position(Other.Position),
    Scale(Other.Scale),
    Rotation(Other.Rotation),
    Parent(Other.Parent)
{
    //Dirty = true;
}

bool CTransform::Load(CXMLElement* Root)
{
    //Dirty = true;
	Position = XML::LoadVector3(Root, "Position", Vector3::ZERO );
	Scale = XML::LoadVector3(Root, "Scale", Vector3::ONE );
    Rotation = XML::LoadQuaternion(Root, "Rotation", Quaternion::IDENTITY);
    return true;
}

bool CTransform::Save(CXMLElement* Root)
{
	XML::SaveVector3(Root, "Position", Position);
	XML::SaveVector3(Root, "Scale", Scale);
	XML::SaveQuaternion(Root, "Rotation", Rotation);
    return true;
}

CTransform& CTransform::operator=(const CTransform& Other)
{
    if( this != &Other )
    {
        Position = Other.Position;
        Scale = Other.Scale;
        Rotation = Other.Rotation;
        Parent = Other.Parent;
        //Dirty = true;
    }
    return *this;
}

void CTransform::SetParent(CTransform* aParent)
{
    Parent = aParent;
}

void CTransform::SetPosition(const Vector3& aPosition, const bool Silent)
{
    Position = aPosition;
    //Dirty = true;
    if (!Silent)
    {    
        for (const auto& i : PositionCallback)
        {
            i.second(Position);
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

void CTransform::SetScale(const Vector3& aScale, const bool Silent)
{
    Scale = aScale;
    //Dirty = true;
    if (!Silent)
    {    
        for (const auto& i : ScaleCallback)
        {
            i.second(Scale);
        }
    }
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

void CTransform::SetRotation(const Quaternion& aRotation, const bool Silent)
{
    Rotation = aRotation;
    //Dirty = true;
    if (!Silent)
    {
        for (const auto& i : RotationCallback)
        {
            i.second(Rotation);
        }
    }
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

Quaternion CTransform::GetWorldRotation() const
{
    Quaternion Tmp = GetRotation();
    if (HasParent())
    {
        Tmp = Parent->GetWorldRotation() * Tmp;
    }
    return Tmp;
}

Quaternion CTransform::GetInvWorldRotation() const
{
    return GetWorldRotation().Inverse();
}

void CTransform::SetForward(const Vector3& NewForward)
{
    SetRotation( Quaternion(Vector3::FORWARD, NewForward) );
}

Vector3 CTransform::GetForward() const
{
    return Rotation * Vector3::FORWARD;
}

Vector3 CTransform::GetWorldForward() const
{
    return GetWorldRotation() * Vector3::FORWARD;
}

void CTransform::SetEulers(const Vector3& Eulers)
{
    SetRotation(Quaternion(Eulers));
}

Vector3 CTransform::GetEulers() const
{
    return Rotation.ToEulerAngles();
}

Vector3 CTransform::GetWorldEulers() const
{
    return GetWorldRotation().ToEulerAngles();
}

void CTransform::Translate(const Vector3& Delta)
{
    SetPosition( Position + Rotation*Delta );
}

void CTransform::Rotate(const Vector3& Eulers)
{
    SetRotation( Rotation*Quaternion(Eulers) );
}

Matrix4 CTransform::GetMatrix() const
{
    return Math::Transform(Position, Rotation, Scale);
    /*if (Dirty)
    {
        Matrix = Math::Scale(Scale)*Math::Rotation(Rotation)*Math::Translation(Position);
        Dirty = false;
    }*/
}

Matrix4 CTransform::GetWorldMatrix() const
{
    Matrix4 Tmp = GetMatrix();
    if (HasParent())
    {
        Tmp = Parent->GetWorldMatrix() * Tmp;
    }
    return Tmp;
}

Matrix4 CTransform::GetInvMatrix() const
{
    return GetMatrix().Inverse();
}

Matrix4 CTransform::GetInvWorldMatrix() const
{
    return GetWorldMatrix().Inverse();
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