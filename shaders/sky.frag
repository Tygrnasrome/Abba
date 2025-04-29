#version 330 core

in vec3 uv;

uniform samplerCube skybox;


layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
void main()
{
    FragColor = texture(skybox, uv);
    BrightColor = vec4(0);
}
