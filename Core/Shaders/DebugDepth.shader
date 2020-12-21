#include "Locations.inc"
#include "Uniforms.inc"

uniform sampler2D Texture;
uniform float Near;
uniform float Far;

// -1 .. 1
float LinearizeDepth1(float d, float zNear, float zFar)
{
    float z_n = 2.0 * d - 1.0;
    return 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
}

// 0 .. 1
// Unity uses this
float LinearizeDepth2(float d, float zNear, float zFar)
{
    return zNear * zFar / (zFar + d * (zNear - zFar));
}

// Unity see LinearEyeDepth
float LinearEyeDepth(float d, float zNear, float zFar)
{
	float x = 1-zFar/zNear;
	float y = zFar/zNear;
	float z = x/zFar;
	float w = y/zFar;
    return 1.0 / (z * d + w);
}

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