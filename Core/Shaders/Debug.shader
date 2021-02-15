#include "Locations.inc"
#include "Uniforms.inc"

#ifdef VS

out vec3 FragPos;
out vec4 ColorVertex;

void main()
{
	gl_Position = ObjectToClipPos(Position);
	FragPos = ObjectToModelPos(Position); 
	ColorVertex = Colors;
}

#endif

#ifdef FS

in vec3 FragPos;
in vec4 ColorVertex;

out vec4 Color;

void main()
{
	Color = vec4(ColorVertex.rgb, 1.0);
}

#endif
