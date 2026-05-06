#version 450 core

layout (location = 0) out vec4 fColour;

in vec2 tex;

uniform sampler2D Texture;

void main()
{
	vec4 tColour=texture(Texture, tex);
	fColour = tColour;
}