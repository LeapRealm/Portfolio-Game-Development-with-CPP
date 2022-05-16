#version 150

out vec4 fragColor;

uniform vec2 sp, ep;
uniform float lineWidth;
uniform vec4 color;

float lineDistance(vec2 s, vec2 e, vec2 p)
{
	vec2 m = p - s;
	vec2 n = e - s;
	
	float len = length(n);
	n /= len;
	
	vec2 proj = clamp(dot(m, n), 0., len) * n;
	
	return length(m - proj);
}

void main()
{
	float d = lineDistance(sp, ep, gl_FragCoord.xy);
	fragColor = vec4(color.rgb, color.a * clamp(lineWidth / 2. - d, 0., 1.));
}
