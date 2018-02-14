#version 330 core

in vec2 uv;

uniform sampler2D texture_sampler;

uniform float time;

out vec4 color;

void main()
{
	color = vec4(texture(texture_sampler, uv + vec2(cos(time), sin(time))).rgb, 1.0);
}