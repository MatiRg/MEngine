#include "Locations.inc"
#include "Uniforms.inc"

// [0, 4]
uniform int SwitchVar = 0;
uniform sampler2D Diffuse;

#ifdef VS

out vec3 VColor;
out vec2 TexCoord;

void main()
{

	VColor	= Colors.rgb;
	TexCoord = TexCoords1;

	gl_Position = Projection * View *Model* vec4(Position, 1.0);
}

#endif

#ifdef FS

out vec4 FragColor;

in vec3 VColor;
in vec2 TexCoord;

float radius = 1.0;
float a = 6.0; 
float b = 20.0;
float c = 5.0;
float d = 5.0;
float e = 5.0;
float f = 5.0;
float h = 0.09;
float i = 0.02;

void main()
{
	switch(SwitchVar)
	{
		case 0:
		{
			//soczewkowaie grawitacyjne z wirem
			//
			//a - szybkość animacji
			//b - kąt obortu

			vec2 coords = TexCoord;
			float dist = distance(coords, vec2(0.5));
			mat2 rotmat;
			if (dist < radius) 
			{
				float percent = (radius - dist) / radius;
				float angle = percent * percent * sin(Time) * b;
				float sina = sin(a*sin(Time)*sqrt(abs(angle)));
				float cosa = cos(sqrt(sin(Time)+1)*angle);
				rotmat = mat2(cosa, sina, -sina, cosa);
			} 
			else 
			{
				rotmat = mat2(1, 0, 0, 1);
			}

			vec2 texCoord = rotmat * (coords - vec2(.5)) + vec2(.5);
			vec4 diffuse = texture2D(Diffuse, texCoord);
			FragColor = diffuse; //* vec4(VColor, 1.0f);
		}break;

		case 1:
		{
			//zwykły wir
			//
			//a - szybkość animacji
			//b - kąt obortu

			vec2 coords = TexCoord;
			float dist = distance(coords, vec2(.5));
			mat2 rotmat;
			if (dist < radius) 
			{
				float percent = (radius - dist) / radius * b;
				float angle = percent * percent * sin(Time) * a;
				float sina = sin(angle);
				float cosa = cos(angle);
				rotmat = mat2(cosa, sina, -sina, cosa);
			} 
			else 
			{
				rotmat = mat2(1, 0, 0, 1);
			}

			vec2 texCoord = rotmat * (coords - vec2(0.5)) + vec2(0.5);
			vec4 diffuse = texture2D(Diffuse, texCoord);
			FragColor = diffuse;// * vec4(VColor, 1.0);
		}break;

		case 2:
		{
			//fala ukośna
			//
			//a - szybkość fali na x
			//b - szybkość fali na y
			//c, d, e, f - modyfikują gęstość fali
			//h, i - wysokość fali
		

			float x = sin( c*TexCoord.y + d*TexCoord.x + sin(Time)*a) * h;
			float y = sin( e*TexCoord.y + f*TexCoord.x + sin(Time)*b) * i;

			FragColor = texture(Diffuse, vec2(TexCoord.x+x, TexCoord.y+y));// * vec4(VColor, 1.0);

		}break;

		case 3:
		{
			//fala płska po Y
			//
			//a - długość fali
			//b - szybkosć fali
			//i - wysokość fali
		
			float y = sin( a*TexCoord.y + sin(Time)*b) * i;

			FragColor = texture(Diffuse, vec2(TexCoord.x, TexCoord.y+y));// * vec4(VColor, 1.0);

		}break;

		case 4:
		{
			//fala płska po X
			//
			//a - długość fali
			//b - szybkosć fali
			//i - wysokość fali
		
			float x = sin( a*TexCoord.x + sin(Time)*b) * i;

			FragColor = texture(Diffuse, vec2(TexCoord.x + x, TexCoord.y));// * vec4(VColor, 1.0);

		}break;

	}	
}

#endif
