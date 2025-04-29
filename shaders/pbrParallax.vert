#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBitangent;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

out vec3 TangentViewPos;
out vec3 TangentFragPos;

//out float gl_ClipDistance[1];

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 viewPos;

void main()
{
    TexCoords = aTexCoords;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(model) * aNormal;

    vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(mat3(model) * aBitangent);
    vec3 N = normalize(mat3(model) * aNormal);
    mat3 TBN = transpose(mat3(T, B, N));

    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;

    gl_Position = projection * view * vec4(FragPos, 1.0);
//    gl_ClipDistance[0] = dot(model * vec4(aPos, 1.0), clipPlane);
}
