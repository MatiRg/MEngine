#define MAX_LIGHTS 8

uniform vec4 AmbientColor = { 0.1, 0.1, 0.1, 1.0 }; 
// 0 to 8
uniform int LightCount;
// 0 - Directional 
// 1 - Point
// 2 - Spot
uniform int LightType[MAX_LIGHTS];
// Color
uniform vec4 LightColor[MAX_LIGHTS];
/*
Directional - [Direction(x,y,z)]
Point - [Position(x,y,z) && Range(w)]
*/
uniform vec4 LightParam1[MAX_LIGHTS];