#include "Locations.inc"
#include "Uniforms.inc"
#ifdef IS_LIT
#include "Lighting.inc"
#endif 

uniform vec4 DiffuseColor = {0.5, 0.5, 0.5, 1.0};
uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;
uniform float Shininess = 16.0;

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

vec3 CalculateDirLight(int index, vec3 viewDir);

void main()
{
	#ifdef IS_LIT
		#ifdef USE_DIFFUSE
		vec3 ObjectColor = texture(DiffuseTexture, TexCoords).rgb;
		#else
		vec3 ObjectColor = DiffuseColor.rgb;
		#endif
		vec3 Ambient = AmbientColor.rgb;
		vec3 Result = Ambient * ObjectColor;
		//
		vec3 ViewDir = GetViewDirection(FragPos);
		for(int i = 0; i < LightCount; i++)
		{
			if( LightType[i] == 0 )
			{
				Result += CalculateDirLight(i, ViewDir);
			}
		}
		//
		Color = vec4(Result, 1.0);
		float Gamma = 1.4;
		Color.rgb = pow(Color.rgb, vec3(1.0/Gamma));
	#else
		#ifdef USE_DIFFUSE
		Color = vec4(texture(DiffuseTexture, TexCoords).rgb, 1.0);
		#else
		Color = vec4(DiffuseColor.rgb, 1.0);
		#endif
	#endif
}

vec3 CalculateDirLight(int index, vec3 viewDir)
{
    vec3 lightDir = normalize(LightParam1[index].xyz); // -LightParam1[index].xyz ?
    // Diffuse shading
    float diff = max(dot(Normal, lightDir), 0.0);
	// blinn-phong
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), Shininess);
	// phong shading
    //vec3 reflectDir = reflect(-lightDir, Normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    // Combine results
    //vec3 Ambient = LightColor[index].rgb * vec3(texture(DiffuseTexture, TexCoords));
	#ifdef USE_DIFFUSE
	vec3 Diffuse = LightColor[index].rgb * diff * vec3(texture(DiffuseTexture, TexCoords));
	#else
	vec3 Diffuse = LightColor[index].rgb * diff * DiffuseColor.rgb;
	#endif
	//
	#ifdef USE_SPECULAR
    vec3 Specular = LightColor[index].rgb * spec * vec3(texture(SpecularTexture, TexCoords));
	#else
	vec3 Specular = LightColor[index].rgb * spec;
	#endif
	return (Diffuse + Specular);
    //return (Ambient + Diffuse + Specular);
}

#endif
