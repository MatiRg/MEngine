#version 330 core

layout (location = 0) in vec3 Position;

uniform mat4 VP;

void main()
{
	gl_Position = vec4(Position, 1.0)*VP;
}
