#include "Locations.inc"
#include "Uniforms.inc"

uniform sampler2D Texture;
uniform vec4 ModColor;

#ifdef VS

out vec2 TexCoords;

void main()
{
	TexCoords = TexCoords1;
	gl_Position = ViewProjection*Model*vec4(Position, 1.0);
}

#endif

#ifdef FS

in vec2 TexCoords;

out vec4 Color;

void main()
{
	Color = texture( Texture, TexCoords )*ModColor;
}

#endif