#include "Transform2D.hpp"
#include "../Core/XML.hpp"

CTransform2D::CTransform2D()
{
}

CTransform2D::CTransform2D(const CTransform2D& Other):
    Position(Other.Position),
    Scale(Other.Scale),
    Angle(Other.Angle),
    Parent(Other.Parent),
    Layer(Other.Layer)
{
}

bool CTransform2D::Load(CXMLElement* Root)
{
	Position = XML::LoadVector2(Root, "Position", Vector2::ZERO() );
	Scale = XML::LoadVector2(Root, "Scale", Vector2::ONE() );
	Angle = XML::LoadFloat(Root, "Angle", 0.0f);
	Layer = XML::LoadFloat(Root, "Layer", 0.0f);
    return true;
}

bool CTransform2D::Save(CXMLElement* Root)
{
	XML::SaveVector2(Root, "Position", Position);
	XML::SaveVector2(Root, "Scale", Scale);
	XML::SaveFloat(Root, "Angle", Angle);
	XML::SaveFloat(Root, "Layer", Layer);
    return true;
}

CTransform2D& CTransform2D::operator=(const CTransform2D& Other)
{
    if( this != &Other )
    {
        Position = Other.Position;
        Scale = Other.Scale;
        Angle = Other.Angle;
        Parent = Other.Parent;
        Layer = Other.Layer;
    }
    return *this;
}

void CTransform2D::SetParent(CTransform2D* aParent)
{
    Parent = aParent;
}

void CTransform2D::SetPosition(const Vector2& aPosition, const bool Silent)
{
    Position = aPosition;
    if( !Silent )
    {
        for(const auto& i: PositionCallback)
        {
            i.second(Position);
        }
    }
}

void CTransform2D::SetWorldPosition(const Vector2& aPosition, const bool Silent)
{
    if( Parent )
    {
        CTransform2D Transform = Parent->GetWorldTransform();
        SetPosition(aPosition-Transform.GetPosition(), Silent);
    }
    else
    {
        SetPosition(aPosition, Silent);
    }
}

void CTransform2D::SetScale(const Vector2& aScale, const bool Silent)
{
    Scale = Vector2( Math::Abs(aScale.x), Math::Abs(aScale.y) );
    if( !Silent )
    {
        for(const auto& i: ScaleCallback)
        {
            i.second(Scale);
        }
    }
}

// Degrees
void CTransform2D::SetAngle(const float aAngle, const bool Silent)
{
    Angle = Math::WrapAngle(aAngle);
    if( !Silent )
    {
        for(const auto& i: AngleCallback)
        {
            i.second(Angle);
        }
    }
}

void CTransform2D::SetWorldAngle(const float aAngle, const bool Silent)
{
    if( Parent )
    {
        CTransform2D Transform = Parent->GetWorldTransform();
        SetAngle(aAngle-Transform.GetAngle(), Silent);
    }
    else
    {
        SetAngle(aAngle, Silent);
    }
}

Vector2 CTransform2D::GetWorldPosition() const
{
    if( HasParent() )
    {
        return Position+Parent->GetWorldPosition();
    }
    return Position;
}

Vector2 CTransform2D::GetWorldScale() const
{
    if( HasParent() )
    {
        return Scale*Parent->GetWorldScale();
    }
    return Scale;
}

float CTransform2D::GetWorldAngle() const
{
    if( HasParent() )
    {
        return Math::WrapAngle( Angle+Parent->GetWorldAngle() );
    }
    return Angle;
}

Vector2 CTransform2D::TransformPoint(const Vector2& Point) const
{
    Vector3 Tmp = Math::Rotation2D( Angle ) * Vector3( Point, 1.0f );
    Vector2 Tmp2 = Vector2( Tmp.x, Tmp.y );
    Tmp2 *= Scale;
    Tmp2 += Position;
    return Tmp2;
}

CTransform2D CTransform2D::GetWorldTransform() const
{
    CTransform2D Tmp = *this;

    if( HasParent() )
    {
        Tmp.Position = Parent->GetWorldTransform().TransformPoint(Position);
        Tmp.Scale = Parent->Scale * Scale;
        Tmp.Angle = Parent->Angle + Angle;
    }
    return Tmp;
}

Matrix3 CTransform2D::GetMatrix() const
{
    return Math::Transform2D(Position, Angle, Scale);
}

void CTransform2D::AddPositionCallback(void* Key, const PositionChanged2D& Callback)
{
    if( !PositionCallback.count(Key) )
    {
        PositionCallback[Key] = Callback;
    }
}

void CTransform2D::AddScaleCallback(void* Key, const ScaleChanged2D& Callback)
{
    if( !ScaleCallback.count(Key) )
    {
        ScaleCallback[Key] = Callback;
    }
}

void CTransform2D::AddAngleCallback(void* Key, const AngleChanged2D& Callback)
{
    if( !AngleCallback.count(Key) )
    {
        AngleCallback[Key] = Callback;
    }
}
