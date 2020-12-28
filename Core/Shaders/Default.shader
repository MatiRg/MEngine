#include "Locations.inc"
#include "Uniforms.inc"
#include "Lighting.inc"

uniform vec4 DiffuseColor = {0.5, 0.5, 0.5, 1.0};
uniform sampler2D DiffuseTexture;

// IS_LIT

#ifdef VS

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
	gl_Position = ObjectToClipPos(Position);
	FragPos = ObjectToModelPos(Position); 
	Normal = ObjectToWorldNormal(Normal1);
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
	#ifdef USE_DIFFUSE
    Color = vec4(texture(DiffuseTexture, TexCoords).rgb, 1.0);
	#else
	Color = vec4(DiffuseColor.rgb, 1.0);
	#endif
}

#endif
