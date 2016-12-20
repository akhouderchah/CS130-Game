#version 330

in vec2 texCoord0;
out vec4 outputColor;

uniform float gAlpha;
uniform sampler2D gSampler;

void main()
{
	outputColor = mix(vec4(0.f, 0.f, 0.f, 0.f), texture2D(gSampler, texCoord0.st), gAlpha);
}

