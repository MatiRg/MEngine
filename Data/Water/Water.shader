#include "Locations.inc"
#include "Uniforms.inc"

uniform vec4 DiffuseColor = {0.5, 0.5, 0.5, 1.0};

#ifdef VS

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
	gl_Position = ObjectToClipPos(Position);
	FragPos = ObjectToModelPos(Position); 
	Normal = ObjectToWorldNormal(Normal1);
	#ifdef FLIP_UVS
	TexCoords = vec2(TexCoords1.x, 1.0 - TexCoords1.y);
	#else
	TexCoords = TexCoords1;
	#endif
}

#endif

#ifdef FS

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 Color;

void main()
{
	Color = vec4(DiffuseColor.rgb, 1.0);
}

#endif
