#include "Locations.inc"
#include "Uniforms.inc"

#ifdef VS

out vec4 OutColors;

void main()
{
	OutColors = Colors;
	gl_Position = ViewProjection*Model*vec4(Position, 1.0);
}

#endif

#ifdef FS

in vec4 OutColors;

out vec4 Color;

void main()
{
	Color = OutColors;
}

#endif