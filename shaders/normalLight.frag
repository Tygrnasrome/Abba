#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 normal;
in vec3 frag_pos;
in vec2 uv;
in vec3 tang;
in vec3 bitang;

uniform mat4 model;

vec3 pos;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform vec2 tileNum;
uniform int opacityThreshold = 0;
float intensity;


layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
uniform sampler2D texture_map;
uniform sampler2D specular_map;
uniform sampler2D opacity_map;
uniform sampler2D normal_map;

void main()
{
	vec3 T = normalize(vec3(model * vec4(tang, 0)));
	vec3 B = normalize(vec3(model * vec4(bitang, 0)));
	vec3 N = normalize(vec3(model * vec4(normal, 0)));
	mat3 TBN = mat3(T,B,N);
	vec3 ambient = light.ambient * vec3(texture2D(texture_map, uv / tileNum));
	vec4 texNormal = texture(normal_map, uv/ tileNum) * 2 - 1;
	vec3 norm = normalize(TBN * texNormal.rgb);
	vec3 lightDir = normalize(light.position - frag_pos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(texture2D(texture_map, uv / tileNum)) * light.diffuse;

	vec3 viewDir = normalize(viewPos - frag_pos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = (diff > 0)? vec3(texture2D(texture_map, uv / tileNum)) * spec *  vec3(texture2D(specular_map, uv / tileNum))*light.specular : vec3(0);


	vec3 result = ambient + specular + diffuse;

	vec4 texColor = texture(opacity_map, uv / tileNum);
	if(opacityThreshold == 1)
		if(texColor.r < 0.1)
		    discard;



	FragColor = vec4(result, 1.0);
	BrightColor = vec4(0);
}
