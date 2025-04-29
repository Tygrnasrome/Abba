#version 330

in vec2 uv;

uniform float angle;
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
uniform vec3 rgb;

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

#define NUM_OCTAVES 5

float fbm(in vec2 st)
{
	float v = 0.0;
	float a = 0.5;
	vec2 shift = vec2(100.0);
	// Rotate to reduce axial bias
	mat2 rot = mat2(cos(0.5), sin(0.5),
			-sin(0.5), cos(0.5));
	for (int i = 0; i < NUM_OCTAVES; ++i)
	{
		v += a * noise(st);
		st = rot * st * 2.0 + shift;
		a *= 0.5;
	}
	return v;
}

void main()
{
	vec2 st = gl_FragCoord.xy / 50.0;
	vec3 color = vec3(0.0);

	vec2 q = vec2(0.);
	q.x = fbm(st + 0.00 * angle);
	q.y = fbm(st + vec2(1.0));

	vec2 r = vec2(0.);
	r.x = fbm(st + 1.0 * q + vec2(1.7, 9.2) + 0.15 * angle);
	r.y = fbm(st + 1.0 * q + vec2(8.3, 2.8) + 0.126 * angle);

	float f = fbm(st + r);

color = mix(rgb,
	       vec3(0.666667,0.666667,0.498039),
	       clamp((f*f)*4.0,0.0,1.0));

   color = mix(color,
	       vec3(rgb),
	       clamp(length(q),0.0,1.0));

   color = mix(color,
	       vec3(rgb),
	       clamp(length(r.x),0.0,1.0));


	FragColor = vec4((f * f * f + .6 * f * f + .5 * f) * color, 0.75);
	BrightColor = vec4(0);
}
