#include "StaticSprite2D.hpp"
#include "../Entity2D.hpp"
#include "../../Graphics/Renderer2D.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Resources/Resources.hpp"
#include "../../Core/XML.hpp"

void CStaticSprite2D::OnRender()
{
    CRenderer2D* Renderer = Engine->GetRenderer2D();
    if( HasTexture() )
    {
        auto Transform = GetOwner()->GetTransform().GetWorldTransform();
        Texture->SetColorMod( ColorMod );
        Renderer->DrawTexture( Texture, Rect2(0.0f, 0.0f, Texture->GetWidth(), Texture->GetHeight()), 
            Rect2( Transform.GetPosition()-Size/2.0f, Size ), Flip, Transform );
    }
}

bool CStaticSprite2D::OnLoad(CXMLElement* Root)
{
    bool HasTexture = XML::LoadBool( Root, "HasTexture", false );
    if( HasTexture )
    {
        std::string TextureName = XML::LoadString( Root, "TextureName", "");
        Texture = Engine->GetResources()->CreateResource<ITexture2D>(TextureName);
        if( !Texture )
        {
            LOG(ESeverity::Error) << "Invalid Texture: " << TextureName << "\n";
            return false;
        }
    }

	Flip = XML::LoadEnum( Root, "Flip", ETextureFlip::None );
    if( Flip < ETextureFlip::None || Flip > ETextureFlip::Both )
    {   
        LOG(ESeverity::Error) << "ETextureFlip Enum Invalid Value\n";
        return false;
    }

	Size = XML::LoadVector2( Root, "Size", Vector2::ONE() );
	ColorMod = XML::LoadColor( Root, "ColorMod", Color::WHITE() );
	Box = XML::LoadRect2( Root, "Box", Rect2(0.0f, 0.0f, 1.0f, 1.0f) );
    return true;
}

bool CStaticSprite2D::OnSave(CXMLElement* Root)
{
    if( HasTexture() )
    {   
        XML::SaveBool( Root, "HasTexture", true);
        XML::SaveString( Root, "TextureName", Texture->GetName());
    }
    else
    {
        XML::SaveBool( Root, "HasTexture", false );
    }
    XML::SaveEnum( Root, "Flip", Flip);
    XML::SaveVector2( Root, "Size", Size);
    XML::SaveColor( Root, "ColorMod", ColorMod);
    XML::SaveRect2( Root, "Box", Box);
    return true;
}

void CStaticSprite2D::SetTexture(ITexture2D* aTexture)
{
    if( !aTexture ) 
    {
        return;
    }
    Texture = aTexture;
}

const Rect2& CStaticSprite2D::GetBox() const
{
    auto Transform = GetOwner()->GetTransform();

    Vector2 WorldSize = Size*Transform.GetWorldScale();
    Box = Rect2( Transform.GetWorldPosition()-WorldSize/2.0f, WorldSize );

    return Box;
}