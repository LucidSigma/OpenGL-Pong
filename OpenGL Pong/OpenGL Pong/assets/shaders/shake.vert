#version 460 core

layout (location = 0) in vec2 in_position;
layout (location = 1) in vec2 in_textureCoordinates;

layout (location = 0) out vec2 v_textureCoordinates;

uniform float u_Time;
uniform bool u_Active;

void main()
{
	v_textureCoordinates = in_textureCoordinates;
	gl_Position = vec4(in_position, 0.0, 1.0);

	if (u_Active)
	{
		const float SHAKE_STRENGTH = 0.01;
		const float SHAKE_MAGNITUDE = 12.5;

		gl_Position.x += cos(u_Time * SHAKE_MAGNITUDE) * SHAKE_STRENGTH;
		gl_Position.y += cos(u_Time * SHAKE_MAGNITUDE) * SHAKE_STRENGTH;
	}	
}