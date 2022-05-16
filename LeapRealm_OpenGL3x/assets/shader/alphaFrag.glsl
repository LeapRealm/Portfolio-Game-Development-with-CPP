#version 150

out vec4 fragColor;

in vec4 colorV;
in vec2 stV;

uniform sampler2D tex;

void main()
{
	fragColor = texture(tex, stV) * colorV;
}
