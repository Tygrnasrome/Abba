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

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D texture_map;
uniform sampler2D texture2_map;

vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec2 mod289(vec2 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec3 permute(vec3 x) { return mod289(((x*34.0)+1.0)*x); }

float snoise(vec2 v) {

    // Precompute values for skewed triangular grid
    const vec4 C = vec4(0.211324865405187,
			// (3.0-sqrt(3.0))/6.0
			0.366025403784439,
			// 0.5*(sqrt(3.0)-1.0)
			-0.577350269189626,
			// -1.0 + 2.0 * C.x
			0.024390243902439);
			// 1.0 / 41.0

    // First corner (x0)
    vec2 i  = floor(v + dot(v, C.yy));
    vec2 x0 = v - i + dot(i, C.xx);

    // Other two corners (x1, x2)
    vec2 i1 = vec2(0.0);
    i1 = (x0.x > x0.y)? vec2(1.0, 0.0):vec2(0.0, 1.0);
    vec2 x1 = x0.xy + C.xx - i1;
    vec2 x2 = x0.xy + C.zz;

    // Do some permutations to avoid
    // truncation effects in permutation
    i = mod289(i);
    vec3 p = permute(
	    permute( i.y + vec3(0.0, i1.y, 1.0))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

    vec3 m = max(0.5 - vec3(
			dot(x0,x0),
			dot(x1,x1),
			dot(x2,x2)
			), 0.0);

    m = m*m ;
    m = m*m ;

    // Gradients:
    //  41 pts uniformly over a line, mapped onto a diamond
    //  The ring size 17*17 = 289 is close to a multiple
    //      of 41 (41*7 = 287)

    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;

    // Normalise gradients implicitly by scaling m
    // Approximation of: m *= inversesqrt(a0*a0 + h*h);
    m *= 1.79284291400159 - 0.85373472095314 * (a0*a0+h*h);

    // Compute final noise value at P
    vec3 g = vec3(0.0);
    g.x  = a0.x  * x0.x  + h.x  * x0.y;
    g.yz = a0.yz * vec2(x1.x,x2.x) + h.yz * vec2(x1.y,x2.y);
    return 130.0 * dot(m, g);
}

void main()
{
	if(snoise(uv*3) < -0.55){
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
		    BrightColor = vec4(0);
	}
	else
		discard;
}
