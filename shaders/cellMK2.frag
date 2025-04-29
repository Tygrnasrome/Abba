#version 330

in vec2 uv;
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
uniform float angle;

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

void main() {

    vec3 color = vec3(.0);


    // Tile the space
    vec2 i_st = floor(uv * 10);
    vec2 f_st = fract(uv * 10);

    float m_dist = 10.;  // minimum distance
    vec2 m_point;        // minimum point

    for (int j=-1; j<=1; j++ ) {
	for (int i=-1; i<=1; i++ ) {
	    vec2 neighbor = vec2(float(i),float(j));
	    vec2 point = random2(i_st + neighbor);
	    point = 0.5 + 0.5*sin(angle + 6.2831*point);
	    vec2 diff = neighbor + point - f_st;
	    float dist = length(diff);

	    if( dist < m_dist ) {
		m_dist = dist;
		m_point = point;
	    }
	}
    }

    // Assign a color using the closest point position
    color += dot(m_point,vec2(.3,.6));

    // Add distance field to closest point center
    // color.g = m_dist;

    // Show isolines
    color -= abs(sin(40.0*m_dist))*0.07;

    // Draw cell center
    color += 1.-step(.05, m_dist);

    FragColor = vec4(color,1.0);
	BrightColor = vec4(0, 0, 0, 0);
}
