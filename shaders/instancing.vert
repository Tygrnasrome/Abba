#version 330

attribute vec3 pos_in;

attribute vec3 color_in;
attribute vec2 uv_in;
uniform mat4 model, view, projection;
varying vec3 color;
varying vec2 uv;
uniform float worldBorder;
float rand(float n)
{
	return fract(sin(n) * 43758.5453123);
}

void main()
{
	float grouping = 5.0;

	vec3 dislocation = vec3( rand(gl_InstanceID/20 / 1000.0) * worldBorder - worldBorder/2 + rand(gl_InstanceID / 1000.0) * grouping - grouping/2, 0,
				 rand(gl_InstanceID/20 % 1000) * worldBorder - worldBorder/2+ rand(gl_InstanceID % 1000) * grouping - grouping/2);
	float alpha = rand(gl_InstanceID % 360)*6.28;
	mat4 rot = mat4(cos(alpha), 0, -sin(alpha), 0,
			0, 1, 0, 0,
			sin(alpha), 0, cos(alpha), 0,
			0, 0, 0, 1);
	mat4 mov = mat4(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			dislocation.x, 0, dislocation.z, 1);
	gl_Position = projection * view * model * mov * rot * vec4(pos_in, 1.0);
	color = color_in;
	uv = uv_in;
}
