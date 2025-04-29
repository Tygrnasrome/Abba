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
in vec2 uv;
in vec3 frag_pos;

vec3 pos;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform vec2 tileNum;
float intensity;

out vec4 FragColor;

uniform sampler2D texture_map;
uniform sampler2D texture2_map;

void main()
{
	vec3 ambient = light.ambient * vec3(texture2D(texture_map, uv * tileNum));

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - frag_pos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(texture2D(texture_map, uv * tileNum)) * light.diffuse;

	vec3 viewDir = normalize(viewPos - frag_pos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = diff > 0 ? vec3(texture2D(texture_map, uv * tileNum)) * spec * light.specular : vec3(0);

	vec3 result = ambient + specular + diffuse;

	FragColor = vec4(result, 1.0);
}
