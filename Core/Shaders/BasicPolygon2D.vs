#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 VSColors;

out vec4 Colors;

uniform mat4 VP;

void main()
{
	Colors = VSColors;
	gl_Position = vec4(Position, 1.0)*VP;
}
