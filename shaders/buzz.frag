#version 330

in vec2 uv;
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D diffuse_map;
uniform vec2 seed;

float random(in vec2 uv)
{
return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453123);
}

float noise(in vec2 uv)
{
vec2 i = floor(uv);
vec2 f = fract(uv);

// Four corners in 2D of a tile
float a = random(i);
float b = random(i + vec2(1.0, 0.0));
float c = random(i + vec2(0.0, 1.0));
float d = random(i + vec2(1.0, 1.0));

vec2 u = f * f * (3.0 - 2.0 * f); // u = smoothstep(0.,1.,f);

return mix(a, b, u.x) +
(c - a) * u.y * (1.0 - u.x) +
(d - b) * u.y * u.x;
}

#define OCTAVES 6


#define NUM_OCTAVES 5

float fbm(in vec2 uv)
{
	float value = 0.0;
	float amplitud = .5;
	float frequency = 0.;

	for(int i = 0; i < OCTAVES; i++)
	{
		value += amplitud * noise(uv);
		uv *= 2.;
		amplitud *= .5;
	}
	return value;
}

float edge(float v, float center, float edge0, float edge1)
{
	return 1.0 - smoothstep(edge0, edge1, abs(v - center));
}

void main()
{
float v0 = edge(fbm((uv + seed) * 18.0), 0.5, 0.0, 0.2);
float v1 = smoothstep(0.5, 0.51, fbm((uv + seed) * 14.0));
float v2 = edge(fbm((uv + seed) * 14.0), 0.5, 0.0, 0.05);
float v3 = edge(fbm((uv + seed) * 14.0), 0.5, 0.0, 0.25);

vec3 col = vec3(1.0);
col -= v0 * 0.75;
col = mix(col, vec3(0.97), v1);
col = mix(col, vec3(0.51), v2);
col -= v3 * 0.2;

FragColor = vec4(vec3(col),1.0);
	BrightColor = vec4(0, 0, 0, 0);
}




