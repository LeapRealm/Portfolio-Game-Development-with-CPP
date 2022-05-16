#version 150

out vec4 fragColor;

uniform vec2 center;
uniform float radius;
uniform vec4 color;

void main()
{
	float r = clamp(radius - length(gl_FragCoord.xy - center.xy), 0., 1.);
	fragColor = color * r;
}
