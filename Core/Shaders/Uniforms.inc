uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 ViewProjection;
uniform vec3 CameraPosition;

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
    return normalize(mat3(transpose(inverse(Model)))*n);
}