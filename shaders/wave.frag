#version 330 core

in vec2 uv;
uniform vec3 viewPos;
in vec3 frag_pos;
in vec4 clipspace;
uniform sampler2D reflectionMap;
uniform sampler2D refractionMap;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
void main(){
	vec2 ndc = clipspace.xy /clipspace.w / 2 + 0.5;
	vec4 reflection = texture(reflectionMap, ndc);
	vec4 refraction = texture(refractionMap, ndc);

	if(reflection.a < 0.1) discard;

	FragColor = mix(reflection, refraction, dot(normalize(viewPos - frag_pos), vec3(0, 1, 0)));
	BrightColor = vec4(0, 0, 0, 0);
}
