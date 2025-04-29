#version 330

attribute vec3 pos_in;

attribute vec3 color_in;
attribute vec2 uv_in;
uniform mat4 model, view, projection;
varying vec3 color;
varying vec2 uv;

void main()
{
	gl_Position = projection * view * model * vec4(pos_in, 1);
	color = color_in;
	uv = uv_in;
}
