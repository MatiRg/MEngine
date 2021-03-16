#include "Locations.inc"
#include "Uniforms.inc"

uniform vec4 DiffuseColor = {0.5, 0.5, 0.5, 1.0};
uniform sampler2D DiffuseTexture;

#ifdef VS

out vec2 TexCoords;

void main()
{
	// Remove Translations
	gl_Position = Projection*RemoveTranslation(View)*RemoveTranslation(Model)*vec4(Position, 1.0);
	#ifdef FLIP_UVS
	TexCoords = TransformUV(TexCoords1);
	#else
	TexCoords = TexCoords1;
	#endif
}

#endif

#ifdef FS

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
