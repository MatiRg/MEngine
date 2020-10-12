#version 330 core

in vec2 TexCoords;

out vec4 Color;

uniform sampler2D Texture;
uniform vec4 ModColor;

void main()
{
	Color = texture( Texture, TexCoords )*ModColor;
}
