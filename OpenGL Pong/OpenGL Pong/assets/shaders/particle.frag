#version 460 core

layout (location = 0) out vec4 out_colour;

layout (location = 0) in vec2 v_textureCoordinates;

uniform sampler2D u_Particle;
uniform float u_Alpha;

void main()
{
	out_colour = texture(u_Particle, v_textureCoordinates);
	out_colour.a *= u_Alpha;
}