#version 330

in vec2 texCoord0;
out vec4 outputColor;

uniform sampler2D gSampler;

void main()
{
	outputColor = texture2D(gSampler, texCoord0.st);
}

