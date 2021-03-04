uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 ViewProjection;
uniform vec3 CameraPosition;
uniform float Time;

vec4 ObjectToClipPos(vec3 p)
{
    return Projection*View*Model*vec4(p, 1.0);
}

vec3 ObjectToViewPos(vec3 p)
{
    return vec3(View*Model*vec4(p, 1.0));
}

vec3 ObjectToModelPos(vec3 p)
{
    return vec3(Model*vec4(p, 1.0));
}

vec3 ObjectToWorldNormal(vec3 n)
{
    // http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/#the-importance-of-the-surface-normal
    mat4 InVM = View*transpose(inverse(Model));
    return (InVM*vec4(n, 0.0)).xyz;
    //return normalize(mat3(transpose(inverse(Model)))*n); // Wrong
}

vec3 GetViewDirection(vec3 f) 
{
    return normalize(CameraPosition - f);
}

//

mat4 RemoveTranslation(mat4 m)
{
    return mat4(mat3(m));
}

//

// -1 .. 1
float LinearizeDepth1(float d, float zNear, float zFar)
{
    float z_n = 2.0 * d - 1.0;
    return 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
}

// 0 .. 1
// Unity uses this ?
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