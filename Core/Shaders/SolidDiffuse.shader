#include "Locations.inc"
#include "Uniforms.inc"

struct Material
{
    sampler2D Diffuse;
    sampler2D Specular;
    float Shininess;
};

struct LightDefinition
{
	int Type; // 0 - Dir; 1 - Point; 2 - Spot
    vec3 Position;
    vec3 Direction;
    float CutOff;
    float OuterCutOff;

    float Constant;
    float Linear;
    float Quadratic;

    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

#define MAX_LIGHTS 8

uniform int LightsCount;
uniform LightDefinition MyLights[MAX_LIGHTS];

uniform Material Material1;

vec3 CalcDirLight(LightDefinition light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(LightDefinition light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(LightDefinition light, vec3 normal, vec3 fragPos, vec3 viewDir);

#ifdef VS

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
	gl_Position = Projection*View*Model*vec4(Position, 1.0);
	FragPos = vec3(Model*vec4(Position, 1.0f));
	Normal = mat3(transpose(inverse(Model)))*Normal1;
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
    // Properties
    vec3 Norm = normalize(Normal);
    vec3 ViewDir = normalize(CameraPosition - FragPos);

	vec3 Result = vec3(0.0);
	for(int i = 0; i < LightsCount; i++)
	{
		if( MyLights[i].Type == 0 )
		{
			Result += CalcDirLight(MyLights[i], Norm, ViewDir);
		}
		else if( MyLights[i].Type == 1 )
		{
			Result += CalcPointLight(MyLights[i], Norm, FragPos, ViewDir);
		}
		else if( MyLights[i].Type == 2 )
		{
			Result += CalcSpotLight(MyLights[i], Norm, FragPos, ViewDir);
		}
	}

    Color = vec4(Result, 1.0);

    float Gamma = 1.4;
    Color.rgb = pow(Color.rgb, vec3(1.0/Gamma));
}

#endif

// Calculates the color when using a Directional light.
vec3 CalcDirLight(LightDefinition light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.Direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
	// blinn-phong
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), Material1.Shininess);
	// phong shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), Material1.Shininess);
    // Combine results
    vec3 Ambient = light.Ambient * vec3(texture(Material1.Diffuse, TexCoords));
    vec3 Diffuse = light.Diffuse * diff * vec3(texture(Material1.Diffuse, TexCoords));
    vec3 Specular = light.Specular * spec * vec3(texture(Material1.Specular, TexCoords));
    return (Ambient + Diffuse + Specular);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(LightDefinition light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.Position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
	// blinn-phong
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), Material1.Shininess);
	// phong shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), Material1.Shininess);
    // Attenuation
    float distance = length(light.Position - fragPos);
    float attenuation = 1.0f / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));
    // Combine results
    vec3 Ambient = light.Ambient * vec3(texture(Material1.Diffuse, TexCoords));
    vec3 Diffuse = light.Diffuse * diff * vec3(texture(Material1.Diffuse, TexCoords));
    vec3 Specular = light.Specular * spec * vec3(texture(Material1.Specular, TexCoords));
    Ambient *= attenuation;
    Diffuse *= attenuation;
    Specular *= attenuation;
    return (Ambient + Diffuse + Specular);
}

// Calculates the color when using a spot light.
vec3 CalcSpotLight(LightDefinition light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.Position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
	// blinn-phong
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), Material1.Shininess);
	// phong shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), Material1.Shininess);
    // Attenuation
    float distance = length(light.Position - fragPos);
    float attenuation = 1.0f / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));
    // Spotlight intensity
    float theta = dot(lightDir, normalize(-light.Direction));
    float epsilon = light.CutOff - light.OuterCutOff;
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);
    // Combine results
    vec3 Ambient = light.Ambient * vec3(texture(Material1.Diffuse, TexCoords));
    vec3 Diffuse = light.Diffuse * diff * vec3(texture(Material1.Diffuse, TexCoords));
    vec3 Specular = light.Specular * spec * vec3(texture(Material1.Specular, TexCoords));
    Ambient *= attenuation * intensity;
    Diffuse *= attenuation * intensity;
    Specular *= attenuation * intensity;
    return (Ambient + Diffuse + Specular);
}