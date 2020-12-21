#include "Drawer2D.hpp"
#include "VertexBuffer.hpp"
#include "Shader.hpp"
#include "Renderable2D.hpp"
#include "Font.hpp"
#include "Window.hpp"
#include "Texture2D.hpp"
#include "../Core/Log.hpp"
#include "../Resources/Resources.hpp"

CDrawer2D::CDrawer2D(IGraphics* aGraphics, CResources* aResources, IWindow* aWindow):
    IEngineModule( "Drawer2D" ),
    Graphics(aGraphics),
    Resources(aResources),
    Window(aWindow)
{
}

CDrawer2D::~CDrawer2D()
{
}

bool CDrawer2D::Init(const SEngineParams&)
{
    ViewProjection = Math::Ortho( static_cast<float>(Window->GetSize().x),
        static_cast<float>(Window->GetSize().y) );

    VertexBuffer = Graphics->CreateVertexBuffer({ {EVertexElement::Position}, {EVertexElement::Color}, {EVertexElement::TexCoord0} }, true);
    if( !VertexBuffer )
    {
        LOG(ESeverity::Fatal) << "Drawer2D: Invalid Vertex Buffer\n";
        return false;
    }

    BasicShader = Resources->CreateResource<IShader>("Basic2D.shader");
    if( !BasicShader )
    {
        LOG(ESeverity::Fatal) << "Drawer2D: Invalid 'Basic2D' Shader\n";
        return false;
    }

    BasicPrimitive = Resources->CreateResource<IShader>("BasicPolygon2D.shader");
    if( !BasicPrimitive )
    {
        LOG(ESeverity::Fatal) << "Drawer2D: Invalid 'BasicPolygon2D' Shader\n";
        return false;
    }

    TextureShader = Resources->CreateResource<IShader>("Texture2D.shader");
    if( !TextureShader )
    {
        LOG(ESeverity::Fatal) << "Drawer2D: Invalid 'Texture2D' Shader\n";
        return false;
    }

    LOG( ESeverity::Info ) << "Drawer2D - Init\n";
    return true;
}

void CDrawer2D::Exit()
{
    VertexBuffer.reset();
    LOG( ESeverity::Info ) << "Drawer2D - Exit\n";
}

void CDrawer2D::DrawPolygon(const std::vector<SVertex2D>& Vertexes, const EPrimitiveMode Type, const float Layer)
{
    DrawPolygon(Vertexes, Type, Matrix4(), Layer);
}

void CDrawer2D::DrawPolygon(const std::vector<SVertex2D>& Vertexes, const EPrimitiveMode Type, const Matrix4& Matrix, const float Layer)
{
    Graphics->SetBlendMode(BlendMode);
    VertexBuffer->Bind();

    std::vector<Vector3> Positions;
    std::vector<Color> Colors;
    for(const auto& i: Vertexes)
    {
        Vector3 Tmp = { i.Position, Layer };
        Positions.push_back( Tmp );

        Colors.push_back( i.DrawColor );
    }
    VertexBuffer->SetData( EVertexElement::Position, Positions );
    VertexBuffer->SetData( EVertexElement::Color, Colors );

    BasicPrimitive->Bind();
    BasicPrimitive->SetMatrix4( "Model", Matrix);
    BasicPrimitive->SetMatrix4( "ViewProjection", ViewProjection );
    VertexBuffer->Draw( Type, Vertexes.size() );
    BasicPrimitive->UnBind();

    VertexBuffer->UnBind();
}

void CDrawer2D::DrawPoint(const Vector2& Position, const Color& DrawColor, const float Layer)
{
    DrawPoint(Position, DrawColor, Matrix4(), Layer);
}

void CDrawer2D::DrawPoint(const Vector2& Position, const Color& DrawColor, const Matrix4& Matrix, const float Layer)
{
    Graphics->SetBlendMode(BlendMode);
    VertexBuffer->Bind();

    std::vector<Vector3> Points = {
        {Position.x, Position.y, Layer}
    };
    VertexBuffer->SetData( EVertexElement::Position, Points );

    BasicShader->Bind();
    BasicShader->SetColor( "OurColor", DrawColor );
    BasicShader->SetMatrix4("Model", Matrix);
    BasicShader->SetMatrix4( "ViewProjection", ViewProjection );
    VertexBuffer->Draw( EPrimitiveMode::Points, 1u );
    BasicShader->UnBind();

    VertexBuffer->UnBind();
}

void CDrawer2D::DrawLine(const Vector2& Start, const Vector2& End, const Color& DrawColor, const float Layer)
{
    DrawLine(Start, End, DrawColor, Matrix4(), Layer);
}

void CDrawer2D::DrawLine(const Vector2& Start, const Vector2& End, const Color& DrawColor, const Matrix4& Matrix, const float Layer)
{
    Graphics->SetBlendMode(BlendMode);
    VertexBuffer->Bind();

    std::vector<Vector3> Points = {
        {Start.x, Start.y, Layer},
        {End.x, End.y, Layer}
    };
    VertexBuffer->SetData( EVertexElement::Position, Points );

    BasicShader->Bind();
    BasicShader->SetColor( "OurColor", DrawColor );
    BasicShader->SetMatrix4("Model", Matrix);
    BasicShader->SetMatrix4( "ViewProjection", ViewProjection );
    VertexBuffer->Draw( EPrimitiveMode::Lines, 2u );
    BasicShader->UnBind();

    VertexBuffer->UnBind();
}

void CDrawer2D::DrawRect(const Rect2& Rectangle, const bool Fill, const Color& DrawColor, const float Layer)
{
    DrawRect(Rectangle, Fill, DrawColor, Matrix4(), Layer);
}

void CDrawer2D::DrawRect(const Rect2& Rectangle, const bool Fill, const Color& DrawColor, const Matrix4& Matrix, const float Layer)
{
    Graphics->SetBlendMode(BlendMode);

    VertexBuffer->Bind();
    if( Fill )
    {
        std::vector<Vector3> Points = {
            { Rectangle.GetX(), Rectangle.GetY(), Layer },
            { Rectangle.GetX(), Rectangle.GetMaxY(), Layer },
            { Rectangle.GetMaxX(), Rectangle.GetY(), Layer },

            { Rectangle.GetMaxX(), Rectangle.GetY(), Layer },
            { Rectangle.GetMaxX(), Rectangle.GetMaxY(), Layer },
            { Rectangle.GetX(), Rectangle.GetMaxY(), Layer }
        };
        VertexBuffer->SetData( EVertexElement::Position, Points );

        BasicShader->Bind();
        BasicShader->SetColor( "OurColor", DrawColor );
        BasicShader->SetMatrix4("Model", Matrix);
        BasicShader->SetMatrix4( "ViewProjection", ViewProjection );
        VertexBuffer->Draw( EPrimitiveMode::Triangles, 6u );
        BasicShader->UnBind();
    }
    else
    {
        std::vector<Vector3> Points = {
            { Rectangle.GetX(), Rectangle.GetY(), Layer },
            { Rectangle.GetX(), Rectangle.GetMaxY(), Layer },

            { Rectangle.GetX(), Rectangle.GetMaxY(), Layer },
            { Rectangle.GetMaxX(), Rectangle.GetMaxY(), Layer },

            { Rectangle.GetMaxX(), Rectangle.GetMaxY(), Layer },
            { Rectangle.GetMaxX(), Rectangle.GetY(), Layer },

            { Rectangle.GetMaxX(), Rectangle.GetY(), Layer },
            { Rectangle.GetX(), Rectangle.GetY(), Layer }
        };
        VertexBuffer->SetData( EVertexElement::Position, Points );

        BasicShader->Bind();
        BasicShader->SetColor( "OurColor", DrawColor );
        BasicShader->SetMatrix4("Model", Matrix);
        BasicShader->SetMatrix4( "ViewProjection", ViewProjection );
        VertexBuffer->Draw( EPrimitiveMode::Lines, 8u );
        BasicShader->UnBind();
    }
    VertexBuffer->UnBind();
}

void CDrawer2D::DrawText(IFont* Font, const std::string& Text, const Vector2& Start, const int Size, const Color& DrawColor, const float Layer)
{
   DrawText(Font, Text, Start, Size, DrawColor, Matrix4(), Layer);
}

void CDrawer2D::DrawText(IFont* Font, const std::string& Text, const Vector2& Start, const int Size, const Color& DrawColor, const Matrix4& Matrix, const float Layer)
{
    if( !Font || !Font->IsValid() )
    {
        LOG( ESeverity::Error ) << "Invalid Font\n";
        return;
    }

    CFontCache* Cache = Font->CreateFontCache( Size );
    if( !Cache )
    {
        LOG( ESeverity::Error ) << "Invalid Font Cache\n";
        return;
    }

    Vector2 Position( Start );
    for(const auto &i: Text)
    {
        if( i == '\n' )
        {
            Position.x = Start.x;
            Position.y += Cache->GetLineHeight();
        }
        else if( Cache->HasGlyph(i) )
        {
            SGlyph* Glyph = Cache->GetGlyph(i);
            ITexture2D* Texture = Cache->GetTexture( Glyph );
            if( !Texture )
            {
                continue;
            }
            Texture->SetColorMod( DrawColor );
            DrawTexture( Texture, Glyph->Rect, {Position, Glyph->Rect.GetSize()}, ETextureFlip::None, Matrix, Layer );
            //
            Position.x += Glyph->Advance;
        }
    }
}

void CDrawer2D::DrawTexture(ITexture2D* Texture, const Rect2& Src, const Rect2& Dst, const ETextureFlip Flip, const float Layer)
{
    DrawTexture(Texture, Src, Dst, Flip, Matrix4(), Layer);
}

void CDrawer2D::DrawTexture(ITexture2D* Texture, const Rect2& Src, const Rect2& Dst, const ETextureFlip Flip, const Matrix4& Matrix, const float Layer)
{
    if( !Texture || !Texture->IsValid() )
    {
        LOG( ESeverity::Error ) << "Invalid Texture\n";
        return;
    }

    Graphics->SetBlendMode(BlendMode);

    float UV_Left = Src.GetX() / static_cast<float>(Texture->GetWidth());
    float UV_Right = Src.GetMaxX() / static_cast<float>(Texture->GetWidth());
    float UV_Top = Src.GetY() / static_cast<float>(Texture->GetHeight());
    float UV_Bottom = Src.GetMaxY() / static_cast<float>(Texture->GetHeight());

    switch(Flip)
    {
    case ETextureFlip::Vertical:
        std::swap(UV_Top, UV_Bottom);
        break;
    case ETextureFlip::Horizontal:
        std::swap(UV_Left, UV_Right);
        break;
    case ETextureFlip::Both:
        std::swap(UV_Left, UV_Right);
        std::swap(UV_Top, UV_Bottom);
        break;
    default:
        break;
    }

    VertexBuffer->Bind();

    std::vector<Vector3> Points = {
        { Dst.GetX(), Dst.GetY(), Layer },
        { Dst.GetX(), Dst.GetMaxY(), Layer },
        { Dst.GetMaxX(), Dst.GetY(), Layer },

        { Dst.GetMaxX(), Dst.GetY(), Layer },
        { Dst.GetMaxX(), Dst.GetMaxY(), Layer },
        { Dst.GetX(), Dst.GetMaxY(), Layer }
    };
    VertexBuffer->SetData( EVertexElement::Position, Points );

    std::vector<Vector2> UVs = {
        { UV_Left, UV_Top },
        { UV_Left, UV_Bottom },
        { UV_Right, UV_Top },

        { UV_Right, UV_Top },
        { UV_Right, UV_Bottom },
        { UV_Left, UV_Bottom },
    };

    VertexBuffer->SetData( EVertexElement::TexCoord0, UVs );

    TextureShader->Bind();
    TextureShader->SetTexture( "Texture", Texture, 0 );
    TextureShader->SetColor( "ModColor", Texture->GetColorMod() );
    TextureShader->SetMatrix4("Model", Matrix);
    TextureShader->SetMatrix4( "ViewProjection", ViewProjection );
    VertexBuffer->Draw(EPrimitiveMode::Triangles, 6u);
    TextureShader->UnBind();

    VertexBuffer->UnBind();
}
