#pragma once
#include "../Math/Matrix3.hpp"
#include "../Math/Vector2.hpp"
#include "../Math/Functions.hpp"
#include <functional>
#include <unordered_map>

class CXMLElement;

using PositionChanged2D = std::function<void(const Vector2&)>;
using ScaleChanged2D = std::function<void(const Vector2&)>;
using AngleChanged2D = std::function<void(const float)>;

class CTransform2D final
{
public:
    CTransform2D();
    CTransform2D(const CTransform2D&);
    ~CTransform2D() = default;

    CTransform2D& operator=(const CTransform2D&);

    bool Load(CXMLElement*);
    bool Save(CXMLElement*);

    void SetParent(CTransform2D*);
    bool HasParent() const { return Parent; }

    // Position, Silent will not call callbacks
    void SetPosition(const Vector2&, const bool Silent = false);
    const Vector2& GetPosition() const { return Position; }

    // Position, Silent will not call callbacks
    void SetWorldPosition(const Vector2&, const bool Silent = false);
    // No Transform Apllied
    Vector2 GetWorldPosition() const;
    
    // Scale, Silent will not call callbacks
    void SetScale(const Vector2&, const bool Silent = false);
    const Vector2& GetScale() const { return Scale; }

    Vector2 GetWorldScale() const;
    
    // Degrees, Silent will not call callbacks
    void SetAngle(const float, const bool Silent = false);
    float GetAngle() const { return Angle; }

    // Degrees, Silent will not call callbacks
    void SetWorldAngle(const float, const bool Silent = false);
    float GetWorldAngle() const;

    void SetLayer(const float aLayer) { Layer = aLayer; }
    float GetLayer() const { return Layer; }

    // Key, Callback
    void AddPositionCallback(void*, const PositionChanged2D&);
    void AddScaleCallback(void*, const ScaleChanged2D&);
    void AddAngleCallback(void*, const AngleChanged2D&);

    // Key
    void RemovePositionCallback(void* Key) { PositionCallback.erase(Key); }
    void RemoveScaleCallback(void* Key) { ScaleCallback.erase(Key); }
    void RemoveAngleCallback(void* Key) { AngleCallback.erase(Key); }

    Vector2 TransformPoint(const Vector2&) const;
    CTransform2D GetWorldTransform() const;

    Matrix3 GetMatrix() const;
private:
    Vector2 Position = Vector2::ZERO(); // Center of Object
    Vector2 Scale = Vector2::ONE();
    float Angle = 0.0f; // Degrees
    CTransform2D* Parent = nullptr;
    float Layer = 0.0f;
    std::unordered_map<void*, PositionChanged2D> PositionCallback;
    std::unordered_map<void*, ScaleChanged2D> ScaleCallback;
    std::unordered_map<void*, AngleChanged2D> AngleCallback;
};