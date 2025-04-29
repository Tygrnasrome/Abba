#version 330 core

layout (location = 0) in vec3 pos_in;
layout (location = 2) in vec2 uv_in;
layout (location = 3) in vec3 normal_in;
layout (location = 4) in vec3 tangent_in;
layout (location = 5) in vec3 bitangent_in;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 frag_pos;
out vec2 uv;
out vec3 tang;
out vec3 bitang;
out mat4 model;
void main()
{

	normal = mat3(transpose(inverse(model))) * normal_in;
	gl_Position = projection * view * model * vec4(pos_in, 1.0);
	frag_pos = vec3(model * vec4(pos_in, 1.0));
	uv = uv_in;
	tang = tangent_in;
	bitang = bitangent_in;
}
