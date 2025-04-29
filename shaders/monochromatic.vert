#version 330 core

layout (location = 0) in vec3 pos_in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 frag_pos;

void main()
{
	gl_Position = projection * view * model * vec4(pos_in, 1.0);
	frag_pos = vec3(model * vec4(pos_in, 1.0));
}
