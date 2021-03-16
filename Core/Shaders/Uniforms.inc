uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 ViewProjection;
uniform vec3 CameraPosition;
uniform vec4 ProjectionParams;
uniform vec4 ZBufferParams;
uniform float Time;
uniform sampler2D DepthTexture; // Only for Transparent Pass

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

vec4 ComputeScreenPos(vec4 pos) 
{
    vec4 o = pos * 0.5;
    o.xy = vec2(o.x, o.y*ProjectionParams.x) + o.w;
    o.zw = pos.zw;
    return o;
}

// For Reading from Framebuffers, etc..
vec2 TransformUV(vec2 i)
{
    return vec2(i.x, 1.0-i.y);
}

//

// -1 .. 1
float LinearizeDepth1(float d)
{
    float z_n = 2.0 * d - 1.0;
    return 2.0 * ProjectionParams.y * ProjectionParams.z / (ProjectionParams.z + ProjectionParams.y - z_n * (ProjectionParams.z - ProjectionParams.y));
}

// 0 .. 1
float LinearizeDepth2(float d)
{
    return ProjectionParams.y * ProjectionParams.z / (ProjectionParams.z + d * (ProjectionParams.y - ProjectionParams.z));
}

// Unity see LinearEyeDepth
float LinearEyeDepth(float d)
{
    return 1.0 / (ZBufferParams.z * d + ZBufferParams.w);
}