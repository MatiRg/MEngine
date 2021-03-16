// https://roystan.net/articles/toon-water.html
#include "Locations.inc"
#include "Uniforms.inc"

uniform vec4 _DepthGradientShallow = {0.325, 0.807, 0.971, 0.725};
uniform vec4 _DepthGradientDeep = {0.086, 0.407, 1.0, 0.749};
uniform float _DepthMaxDistance = 1.0;

#ifdef VS

out vec4 ScreenPosition;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
	gl_Position = ObjectToClipPos(Position);
	ScreenPosition = ComputeScreenPos(gl_Position); 
	Normal = ObjectToWorldNormal(Normal1);
	#ifdef FLIP_UVS
	TexCoords = TransformUV(TexCoords1);
	#else
	TexCoords = TexCoords1;
	#endif
}

#endif

#ifdef FS

in vec4 ScreenPosition;
in vec3 Normal;
in vec2 TexCoords;

out vec4 Color;

void main()
{
	float existingDepth01 = texture(DepthTexture, TransformUV(ScreenPosition.xy/ScreenPosition.w)).r;
	float existingDepthLinear = LinearEyeDepth(existingDepth01);
	float depthDifference = existingDepthLinear - ScreenPosition.w;

	float waterDepthDifference01 = clamp(depthDifference / _DepthMaxDistance, 0.0, 1.0);
	Color = mix(_DepthGradientShallow, _DepthGradientDeep, waterDepthDifference01);
}

#endif
