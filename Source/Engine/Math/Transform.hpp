#pragma once
#include "../Core/NonCopyable.hpp"
#include "Matrix4.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"
#include "Functions.hpp"
#include <functional>
#include <vector>

class CXMLElement;
class CTransform;

// Local Space
using PositionChanged = std::function<void(const Vector3&)>;
// Local Space
using ScaleChanged = std::function<void(const Vector3&)>;
// Local Space
using RotationChanged = std::function<void(const Quaternion&)>;
//! Is Always Fired, when marked Dirty
using TransformChanged = std::function<void(CTransform*)>;

using TransformArray = std::vector<CTransform*>;

/**
  \class CTransform
  \brief Encapsulate Position, Rotation and Scale.Tree Like structure.
*/
class CTransform final: public NonCopyableMovable
{
public:
    CTransform();
    ~CTransform();

    bool Load(CXMLElement* Root);
    bool Save(CXMLElement* Root);

    void SetParent(CTransform* aParent);
    //
    bool HasParent() const { return Parent; }
    CTransform* GetParent() const { return Parent; }

    void AddChild(CTransform*);
    void RemoveChild(CTransform*);
    //
    const TransformArray& GetChildren() const { return Children; }
    //
    bool HasChildren() const { return !Children.empty(); }
    std::size_t GetChildrenCount() const { return Children.size(); }

    //! Local Space
    void SetPosition(const Vector3& aPosition, const bool Silent = false);
    //! Local Space
    const Vector3& GetPosition() const { return Position; }
    //! Local Space
    void SetScale(const Vector3& aScale, const bool Silent = false);
    //! Local Space
    const Vector3& GetScale() const { return Scale; }
    //! Local Space
    void SetRotation(const Quaternion& aRotation, const bool Silent = false);
    //! Local Space
    const Quaternion& GetRotation() const { return Rotation; }

    //! World Space
    void SetWorldPosition(const Vector3& aPosition, const bool Silent = false);
    //! World Space
    Vector3 GetWorldPosition() const;
    //! World Space
    void SetWorldScale(const Vector3& aScale, const bool Silent = false);
    //! World Space
    Vector3 GetWorldScale() const;
    //! World Space
    void SetWorldRotation(const Quaternion& aRotation, const bool Silent = false);
    //! World Space
    const Quaternion& GetWorldRotation() const;
    //! World Space
    const Quaternion& GetInvWorldRotation() const;

    //! World Space
    void SetRight(const Vector3&);
    //! World Space
    Vector3 GetRight() const;
    //! World Space
    void SetUp(const Vector3&);
    //! World Space
    Vector3 GetUp() const;
    //! World Space
    void SetForward(const Vector3&);
    //! World Space
    Vector3 GetForward() const;

    //! Local Space, Degrees
    void SetEulerAngles(const Vector3&);
    //! Local Space, Degrees
    Vector3 GetEulerAngles() const;
    //! World Space, Degrees
    void SetWorldEulerAngles(const Vector3&);
    //! World Space, Degrees
    Vector3 GetWorldEulerAngles() const;

    //! Local Space
    void Translate(const Vector3&);
    //! Eulers Degrees, Local Space
    void Rotate(const Vector3& Eulers) { Rotate( Quaternion(Eulers) ); }
    //! Eulers Degrees, Local Space
    void Rotate(const float x, const float y, const float z) { Rotate( Quaternion(x, y, z) ); }
    //! Local Space
    void Rotate(const Quaternion&);

    // World Space
    void LookAt(CTransform* Target, const Vector3& Up = Vector3::UP) { LookAt(Target->GetWorldPosition(), Up); }
    // World Space
    void LookAt(const CTransform& Target, const Vector3& Up = Vector3::UP) { LookAt(Target.GetWorldPosition(), Up); }
    // World Space
    void LookAt(const Vector3& Target, const Vector3& Up = Vector3::UP);

    // World Space, Degrees
    void RotateAround(CTransform* Point, const Vector3& Axis, const float Angle) { RotateAround(Point->GetWorldPosition(), Axis, Angle); }
    // World Space, Degrees
    void RotateAround(const CTransform& Point, const Vector3& Axis, const float Angle) { RotateAround(Point.GetWorldPosition(), Axis, Angle); }
    // World Space, Degrees
    void RotateAround(const Vector3& Point, const Vector3& Axis, const float Angle);

    Vector3 LocalToWorld(const Vector3& Point) { return GetWorldMatrix() * Point; }
    Vector3 WorldToLocal(const Vector3& Point) { return GetInvWorldMatrix() * Point; }

    const Matrix4& GetMatrix() const;
    const Matrix4& GetInvMatrix() const;

    // Can be Used to Convert Local to World and For Rendering
    const Matrix4& GetWorldMatrix() const;
    // Can be Used to Convert World to Local
    const Matrix4& GetInvWorldMatrix() const;

    // Key, Callback
    void AddPositionCallback(void* Key, const PositionChanged& Callback);
    void AddScaleCallback(void* Key, const ScaleChanged& Callback);
    void AddRotationCallback(void* Key, const RotationChanged& Callback);
    void AddChangedCallback(void* Key, const TransformChanged& Callback);

    // Key
    void RemovePositionCallback(void* Key);
    void RemoveScaleCallback(void* Key);
    void RemoveRotationCallback(void* Key);
    void RemoveChangedCallback(void* Key);

    bool IsDirty() const { return Dirty; }

    // 2D Functions Used by 2D Components

    void SetPosition2D(const Vector2& aPosition, const bool Silent = false) { SetPosition({ aPosition, GetPosition().z }, Silent); }
    Vector2 GetPosition2D() const { return GetPosition().XY(); }
    void SetScale2D(const Vector2& aScale, const bool Silent = false) { SetScale({ aScale, 1.0f }, Silent); }
    Vector2 GetScale2D() const { return GetScale().XY(); }
    void SetRotation2D(const float aAngle, const bool Silent = false) { SetRotation(Quaternion({ 0.0f, 0.0f, 1.0f }, aAngle), Silent); }
    float GetRotation2D() const { return Rotation.ToEulerAngles().z; }

    void SetWorldPosition2D(const Vector2& aPosition, const bool Silent = false) { SetWorldPosition({ aPosition, GetPosition().z }, Silent); }
    Vector2 GetWorldPosition2D() const { return GetWorldPosition().XY(); }
    void SetWorldScale2D(const Vector2& aScale, const bool Silent = false) { SetWorldScale({ aScale, 1.0f }, Silent); }
    Vector2 GetWorldScale2D() const { return GetWorldScale().XY(); }
    void SetWorldRotation2D(const float aAngle, const bool Silent = false) { SetWorldRotation(Quaternion({ 0.0f, 0.0f, 1.0f }, aAngle), Silent); }
    float GetWorldRotation2D() const { return GetWorldRotation().ToEulerAngles().z; }

    void SetLayer(const float aLayer, const bool Silent = false) { SetPosition({ GetPosition().XY(), aLayer }, Silent); }
    float GetLayer() const { return GetPosition().z; }
private:
    void MarkDirty(); 
    void RecalculateMatrix() const;
private:
    Vector3 Position = Vector3::ZERO;
    Vector3 Scale = Vector3::ONE;
    Quaternion Rotation = Quaternion::IDENTITY;
    CTransform* Parent = nullptr;
    TransformArray Children;
    // Cache, mutable is used to be modified from const methods. These variables work as cache only
    mutable bool Dirty = true;
    mutable Quaternion WorldRotation = Quaternion::IDENTITY;
    mutable Quaternion InvWorldRotation = Quaternion::IDENTITY;
    mutable Matrix4 Matrix = Matrix4::IDENTITY;
    mutable Matrix4 InvMatrix = Matrix4::IDENTITY;
    mutable Matrix4 WorldMatrix = Matrix4::IDENTITY;
    mutable Matrix4 InvWorldMatrix = Matrix4::IDENTITY;
    std::vector<std::pair<void*, PositionChanged>> PositionCallback;
    std::vector<std::pair<void*, ScaleChanged>> ScaleCallback;
    std::vector<std::pair<void*, RotationChanged>> RotationCallback;
    std::vector<std::pair<void*, TransformChanged>> TransformChangedCallback;
};