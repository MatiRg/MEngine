#include "Locations.inc"
#include "Uniforms.inc"

uniform vec4 DiffuseColor;

#ifdef VS

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
	gl_Position = Projection*View*Model*vec4(Position, 1.0);
	FragPos = vec3(Model*vec4(Position, 1.0f));
	Normal = mat3(transpose(inverse(Model)))*Normal1;
	TexCoords = TexCoords1;
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
