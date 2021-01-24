#include "Locations.inc"
#include "Uniforms.inc"
#include "PostEffect.inc"

uniform int Samples;

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

const float BlurSizeH = 1.0 / 600.0;
const float BlurSizeV = 1.0 / 600.0;

void main()
{
	float Pow = (Samples*2+1) * (Samples*2+1);
    vec4 Sum = vec4(0.0);
    for (int x = -Samples; x <= Samples; x++)
    {    
		for (int y = -Samples; y <= Samples; y++)
        {    
			Sum += texture(BackBufferColor,
                vec2(TexCoords.x + x * BlurSizeH, TexCoords.y + y * BlurSizeV)
            ) / Pow;
		}
	}	
    Color = Sum;
}

#endif