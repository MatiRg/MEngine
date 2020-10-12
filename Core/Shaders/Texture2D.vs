#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 2) in vec2 TexCoords1;

out vec2 TexCoords;

uniform mat4 VP;

void main()
{
	TexCoords = TexCoords1;
	gl_Position = vec4(Position, 1.0)*VP;
}
