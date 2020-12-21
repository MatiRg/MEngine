#include "Locations.inc"
#include "Uniforms.inc"

uniform vec4 OurColor;

#ifdef VS

void main()
{
	gl_Position = ViewProjection*Model*vec4(Position, 1.0);
}

#endif

#ifdef FS

out vec4 Color;

void main()
{
	Color = OurColor;
}

#endif