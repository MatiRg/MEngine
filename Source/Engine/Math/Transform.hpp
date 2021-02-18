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

using PositionChanged = std::function<void(const Vector3&)>;
using ScaleChanged = std::function<void(const Vector3&)>;
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

    void SetPosition(const Vector3& aPosition, const bool Silent = false);
    const Vector3& GetPosition() const { return Position; }

    void SetWorldPosition(const Vector3& aPosition, const bool Silent = false);
    Vector3 GetWorldPosition() const;
    
    void SetScale(const Vector3& aScale, const bool Silent = false);
    const Vector3& GetScale() const { return Scale; }

    // ?
    void SetWorldScale(const Vector3& aScale, const bool Silent = false);
    // ?
    Vector3 GetWorldScale() const;

    void SetRotation(const Quaternion& aRotation, const bool Silent = false);
    const Quaternion& GetRotation() const { return Rotation; }

    void SetWorldRotation(const Quaternion& aRotation, const bool Silent = false);
    Quaternion GetWorldRotation() const;
    Quaternion GetInvWorldRotation() const;

    // ? World Space
    void SetRight(const Vector3&);
    // ? World Space
    Vector3 GetRight() const;

    // ? World Space
    void SetUp(const Vector3&);
    // ? World Space
    Vector3 GetUp() const;

    // ? World Space
    void SetForward(const Vector3&);
    // ? World Space
    Vector3 GetForward() const;

    // World Space
    void SetEulers(const Vector3&);
    // World Space
    Vector3 GetEulers() const;

    // Local Space
    void Translate(const Vector3&);
    //! Eulers, Local Space
    void Rotate(const Vector3&);
    //! Eulers, Local Space
    void Rotate(const float x, const float y, const float z) { Rotate({x, y, z}); }

    const Matrix4& GetMatrix() const;
    const Matrix4& GetInvMatrix() const;

    const Matrix4& GetWorldMatrix() const;
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

    void SetPosition2D(const Vector2& aPosition, const bool Silent = false) { SetPosition({ aPosition, Position.z }, Silent); }
    Vector2 GetPosition2D() const { return GetPosition().XY(); }

    void SetWorldPosition2D(const Vector2& aPosition, const bool Silent = false) { SetWorldPosition({ aPosition, Position.z }, Silent); }
    Vector2 GetWorldPosition2D() const { return GetWorldPosition().XY(); }

    void SetScale2D(const Vector2& aScale, const bool Silent = false) { SetScale({ aScale, 1.0f }, Silent); }
    Vector2 GetScale2D() const { return GetScale().XY(); }

    void SetWorldScale2D(const Vector2& aScale, const bool Silent = false) { SetWorldScale({ aScale, 1.0f }, Silent); }
    Vector2 GetWorldScale2D() const { return GetWorldScale().XY(); }

    void SetRotation2D(const float aAngle, const bool Silent = false) { SetRotation(Quaternion({ 0.0f, 0.0f, 1.0f }, aAngle), Silent); }
    float GetRotation2D() const { return Rotation.ToEulerAngles().z; }

    void SetWorldRotation2D(const float aAngle, const bool Silent = false) { SetWorldRotation(Quaternion({ 0.0f, 0.0f, 1.0f }, aAngle), Silent); }
    float GetWorldRotation2D() const { return GetWorldRotation().ToEulerAngles().z; }

    void SetLayer(const float aLayer) { Position.z = aLayer; }
    float GetLayer() const { return Position.z; }
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
    mutable Matrix4 Matrix;
    mutable Matrix4 InvMatrix;
    mutable Matrix4 WorldMatrix;
    mutable Matrix4 InvWorldMatrix;
    std::vector<std::pair<void*, PositionChanged>> PositionCallback;
    std::vector<std::pair<void*, ScaleChanged>> ScaleCallback;
    std::vector<std::pair<void*, RotationChanged>> RotationCallback;
    std::vector<std::pair<void*, TransformChanged>> TransformChangedCallback;
};