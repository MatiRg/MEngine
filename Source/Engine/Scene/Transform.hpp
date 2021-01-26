#pragma once
#include "../Math/Matrix4.hpp"
#include "../Math/Vector3.hpp"
#include "../Math/Quaternion.hpp"
#include "../Math/Functions.hpp"
#include <functional>
#include <vector>

class CXMLElement;

using PositionChanged = std::function<void(const Vector3&)>;
using ScaleChanged = std::function<void(const Vector3&)>;
using RotationChanged = std::function<void(const Quaternion&)>;

/**
  \class CTransform
  \brief Encapsulate Position, Rotation and Scale.Tree Like structure.
*/
class CTransform final
{
public:
    CTransform();
    CTransform(const CTransform& Other);
    ~CTransform() = default;

    CTransform& operator=(const CTransform& Other);

    bool Load(CXMLElement* Root);
    bool Save(CXMLElement* Root);

    void SetParent(CTransform* aParent);
    bool HasParent() const { return Parent; }
    CTransform* GetParent() const { return Parent; }

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

    // ?
    void SetForward(const Vector3&);
    Vector3 GetForward() const;
    Vector3 GetWorldForward() const;

    // 2D
    void SetPosition2D(const Vector2& aPosition, const bool Silent = false) { SetPosition({ aPosition, Position.z }, Silent); }
    Vector2 GetPosition2D() const { return GetPosition().XY(); }

    void SetWorldPosition2D(const Vector2& aPosition, const bool Silent = false) { SetWorldPosition({ aPosition, Position.z }, Silent); }
    Vector2 GetWorldPosition2D() const { return GetWorldPosition().XY(); }

    void SetScale2D(const Vector2& aScale, const bool Silent = false) { SetScale( {aScale, 1.0f}, Silent ); }
    Vector2 GetScale2D() const { return GetScale().XY(); }

    void SetWorldScale2D(const Vector2& aScale, const bool Silent = false) { SetWorldScale({ aScale, 1.0f }, Silent); }
    Vector2 GetWorldScale2D() const { return GetWorldScale().XY(); }

    void SetRotation2D(const float aAngle, const bool Silent = false) { SetRotation(Quaternion({ 0.0f, 0.0f, 1.0f}, aAngle ), Silent); }
    float GetRotation2D() const { return Rotation.ToEulerAngles().z; }

    void SetWorldRotation2D(const float aAngle, const bool Silent = false) { SetWorldRotation(Quaternion({ 0.0f, 0.0f, 1.0f }, aAngle), Silent); }
    float GetWorldRotation2D() const { return GetWorldRotation().ToEulerAngles().z; }

    void SetLayer(const float aLayer) { Position.z = aLayer; }
    float GetLayer() const { return Position.z; }

    //

    void Translate(const Vector3&);

    Matrix4 GetMatrix() const;
    Matrix4 GetInvMatrix() const;

    Matrix4 GetWorldMatrix() const;
    Matrix4 GetInvWorldMatrix() const;

    // Key, Callback
    void AddPositionCallback(void* Key, const PositionChanged& Callback);
    void AddScaleCallback(void* Key, const ScaleChanged& Callback);
    void AddRotationCallback(void* Key, const RotationChanged& Callback);

    // Key
    void RemovePositionCallback(void* Key);
    void RemoveScaleCallback(void* Key);
    void RemoveRotationCallback(void* Key);
private:
    Vector3 Position = Vector3::ZERO;
    Vector3 Scale = Vector3::ONE;
    Quaternion Rotation = Quaternion::IDENTITY;
    CTransform* Parent = nullptr;
    //mutable bool Dirty = true;
    std::vector<std::pair<void*, PositionChanged>> PositionCallback;
    std::vector<std::pair<void*, ScaleChanged>> ScaleCallback;
    std::vector<std::pair<void*, RotationChanged>> RotationCallback;
};