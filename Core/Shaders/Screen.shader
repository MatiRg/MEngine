#include "Locations.inc"
#include "Uniforms.inc"

uniform sampler2D ScreenTexture;
uniform float GammaCorrection = 1.4;

#ifdef VS

out vec2 TexCoords;

void main()
{
	TexCoords = TexCoords1;
	gl_Position = vec4(Position, 1.0);
}

#endif

#ifdef FS

in vec2 TexCoords;

out vec4 Color;

void main()
{
	vec3 Tmp = texture( ScreenTexture, TexCoords ).rgb;
	Tmp = pow(Tmp, vec3(1.0/GammaCorrection));
	Color = vec4(Tmp, 1.0);
}

#endif