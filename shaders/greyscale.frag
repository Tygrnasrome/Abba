#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float time;

float random(vec2 uv){
    //return fract( sin( p / 0.1 ) * 9999 );
     //return fract(sin(p)*43758.5453);
     return fract(sin(dot(uv,vec2(12.9898,78.233)))*43758.5453123);
}

void main()
{
    vec2 uv;

    FragColor = texture(screenTexture, TexCoords);
    float grey = (0.212 * FragColor.x + 0.715 * FragColor.y + 0.0722 * FragColor.z)/1;
    uv.x = round(TexCoords.x * 200) / 200.0;
    uv.y = round(TexCoords.y * 200) / 200.0;
    FragColor = mix(vec4(grey,grey,grey,0) , vec4(random(uv * time), random(uv * time), random(uv * time), 0),0.25);
	BrightColor = vec4(0, 0, 0, 0);
}
