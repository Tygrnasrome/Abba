#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform vec3 color;
void main()
{
	FragColor = vec4(color, 0.5);
	BrightColor = vec4(0,0,0, 0);
}
