#include "VertexBuffer.hpp"

SVertexElement::SVertexElement(const EVertexElement aElement):
    Element(aElement)
{
    switch (Element)
    {
    case EVertexElement::Position:
        Type = EVertexType::Vector3;
        Size = sizeof(Vector3);
		ElementSize = 3u;
        break;
    case EVertexElement::Color:
        Type = EVertexType::Color;
        Size = sizeof(Color);
		ElementSize = 4u;
        break;
    case EVertexElement::TexCoord0:
        Type = EVertexType::Vector2;
        Size = sizeof(Vector2);
		ElementSize = 2u;
        break;
    case EVertexElement::Normal:
        Type = EVertexType::Vector3;
        Size = sizeof(Vector3);
		ElementSize = 3u;
        break;
    default:
        break;
    }
}