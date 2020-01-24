#version 460 core

layout (location = 0) out vec4 out_colour;

layout (location = 0) in vec2 v_textureCoordinates;

uniform sampler2D u_Frame;

uniform vec2 u_KernelOffsets[9];
uniform float u_BlurKernel[9];
uniform bool u_Active;

void main()
{
	out_colour = vec4(0.0);
	
	if (u_Active)
	{
		for (uint i = 0; i < 9; ++i)
		{
			vec3 frameSample = vec3(texture(u_Frame, v_textureCoordinates + u_KernelOffsets[i]));
			out_colour += vec4(frameSample * u_BlurKernel[i], 0.0);
		}

		out_colour.a = 1.0;
	}
	else
	{
		out_colour = texture(u_Frame, v_textureCoordinates);
	}
}