#include "Locations.inc"
#include "Uniforms.inc"

uniform sampler2D Texture;
uniform float Near;
uniform float Far;

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
	float r = LinearEyeDepth( texture( Texture, TexCoords ).r, Near, Far );
	vec3 val = vec3(r, r, r);
	Color = vec4(val, 1.0);
}

#endif