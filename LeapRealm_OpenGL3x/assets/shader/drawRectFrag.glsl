#version 150

out vec4 fragColor;

uniform vec2 u_position;
uniform vec2 u_size;
uniform float u_radius;
uniform float u_lineWidth;
uniform float u_radian;
uniform vec4 u_color;

vec2 rotate(vec2 p, float a)
{
//  ccw
	mat2 m = mat2(cos(a), -sin(a), -sin(a), cos(a));

//  cw
//	mat2 m = mat2(cos(a), -sin(a), sin(a), cos(a));
	return p * m;
}

float rectDistance(vec2 p, vec2 size, float radius)
{
	size -= vec2(radius);
	vec2 d = abs(p) - size;
	return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - radius;
}

float alpha(float d, float width)
{
	float alpha1 = clamp(d + width, 0.0, 1.0);
	float alpha2 = clamp(d, 0.0, 1.0);
	
	return alpha1 - alpha2;
}

void main()
{
	if(u_radian == 0.0)
	{
		float d = rectDistance(gl_FragCoord.xy - u_position, u_size, u_radius);
		fragColor = vec4(u_color.rgb, u_color.a * alpha(d, u_lineWidth));
	}
	else
	{
		vec2 p = rotate(gl_FragCoord.xy - u_position, u_radian);
		float d = rectDistance(p, u_size, u_radius);
		fragColor = vec4(u_color.rgb, u_color.a * alpha(d, u_lineWidth));
	}
}
