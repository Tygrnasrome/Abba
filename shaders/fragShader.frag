#version 450

in vec2 uv;
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D diffuse_map;

void main()
{
    vec4 texColor = texture(diffuse_map, uv);
    if(texColor.a < 0.1)
	discard;
    FragColor = texColor;
	BrightColor = vec4(0, 0, 0, 0);
}
