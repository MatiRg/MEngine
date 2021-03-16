// Based on ChromaticAberration.fx from ReShade
// First Includes 
#include "Locations.inc" // Vertex Input
#include "Uniforms.inc" // Typical Uniforms
#include "PostEffect.inc" // Post Process Uniforms

// Uniforms
// -10 to 10
uniform vec2 Shift = {2.5, -0.5};
// 0 to 1
uniform float Strength = 0.5;

// Vertex Shader
#ifdef VS

out vec2 TexCoords;

void main()
{
	TexCoords = TexCoords1;
	gl_Position = vec4(Position, 1.0);
}

#endif

// Fragment Shader
#ifdef FS

in vec2 TexCoords;

out vec4 Color;

void main()
{	
    vec3 InputColor = texture(BackbufferColor, TexCoords).rgb;
    vec3 Tmp = texture(BackbufferColor, TexCoords).rgb;

	// Sample the color components
	Tmp.r = texture(BackbufferColor, TexCoords + (PixelSize * Shift)).r;
	Tmp.g = InputColor.g;
	Tmp.b = texture(BackbufferColor, TexCoords - (PixelSize * Shift)).b;
	// Adjust the strength of the effect
	Tmp = mix(InputColor, Tmp, Strength);

    Color = vec4(Tmp, 1.0);
}

#endif