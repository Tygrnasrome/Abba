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
float heightScale;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D texture_map;
uniform sampler2D specular_map;
uniform sampler2D opacity_map;
uniform sampler2D normal_map;
uniform sampler2D height_map;

vec2 ParallaxMapping( vec3 viewDir, vec2 texCoords)
{
	float height_scale = 0.08;
	// number of depth layers
	const float minLayers = 8.0;
	const float maxLayers = 32.0;
	float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));
	// calculate the size of each layer
	float layerDepth = 1.0 / numLayers;
	// depth of current layer
	float currentLayerDepth = 0.0;
	// the amount to shift the texture coordinates per layer (from vector P)
	vec2 P = viewDir.xy * height_scale;
	vec2 deltaTexCoords = P / numLayers;

	// get initial values
	vec2  currentTexCoords     = texCoords;
	float currentDepthMapValue = 1 - texture(height_map, currentTexCoords).r;

	while(currentLayerDepth < currentDepthMapValue)
	{
	// shift texture coordinates along direction of P
	currentTexCoords -= deltaTexCoords;
	// get depthmap value at current texture coordinates
	currentDepthMapValue = 1 - texture(height_map, currentTexCoords).r;
	// get depth of next layer
	currentLayerDepth += layerDepth;
	}

	// get texture coordinates before collision (reverse operations)
	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

	// get depth after and before collision for linear interpolation
	float afterDepth  = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = 1 - texture(height_map, prevTexCoords).r - currentLayerDepth + layerDepth;

	// interpolation of texture coordinates
	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
	if(finalTexCoords.r > 1 || finalTexCoords.r < 0 || finalTexCoords.g < 0 || finalTexCoords.g > 1)
		discard;
	else
		return finalTexCoords;
}

vec2 transformUv(vec3 viewDir, vec2 uv)
{
	heightScale = 0.05;
	float h = 1 - texture2D(height_map, uv * tileNum).r;
	vec2 p = viewDir.xy/viewDir.z * h * heightScale;
	return uv - p;
}

void main()
{
	vec3 viewDir = normalize(viewPos - frag_pos);
	vec3 T = normalize(vec3(model * vec4(tang, 0)));
	vec3 B = normalize(vec3(model * vec4(bitang, 0)));
	vec3 N = normalize(vec3(model * vec4(normal, 0)));
	mat3 TBN = mat3(T,B,N);
	//vec2 scaledUv = transformUv(normalize(TBN*viewDir), uv * tileNum);
	vec2 scaledUv = ParallaxMapping(normalize(TBN*viewDir), uv * tileNum);
	vec3 ambient = light.ambient * vec3(texture2D(texture_map, scaledUv));
	vec4 texNormal = texture(normal_map, scaledUv) * 2 - 1;
	vec3 norm = normalize(TBN * texNormal.rgb);
	vec3 lightDir = normalize(light.position - frag_pos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(texture2D(texture_map, scaledUv)) * light.diffuse;


	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = (diff > 0)? vec3(texture2D(texture_map, scaledUv)) * spec *  vec3(texture2D(specular_map, scaledUv))*light.specular : vec3(0);


	vec3 result = ambient + specular + diffuse;

	vec4 texColor = texture(opacity_map, scaledUv);
	if(opacityThreshold == 1)
		if(texColor.r < 0.1)
		    discard;

	FragColor = vec4(result, 1.0);
	BrightColor = vec4(0,0,0,0)

}
