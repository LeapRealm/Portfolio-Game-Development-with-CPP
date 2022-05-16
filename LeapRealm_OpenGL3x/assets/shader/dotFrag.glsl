#version 150

out vec4 fragColor;

uniform vec2 center;
uniform float dotSize;
uniform vec4 color;

void main()
{
	float d = length(gl_FragCoord.xy - center.xy);
	fragColor = vec4(color.rgb, color.a * clamp(dotSize/2. - d, 0., 1.));
}
