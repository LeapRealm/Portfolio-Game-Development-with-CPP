#if 0
	uniform vec3      iResolution;				
	uniform float     iTime;					
	uniform float     iTimeDelta;				
	uniform int       iFrame;              		
	uniform float     iChannelTime[4];			
	uniform vec3      iChannelResolution[4];	
	uniform vec4      iMouse;					
	uniform sampler2D iChannel0;        		
	uniform sampler2D iChannel1;        		
	uniform sampler2D iChannel2;        		
	uniform sampler2D iChannel3;        		
	uniform vec4      iDate;					
	uniform float     iSampleRate;				
#endif


float lineDistance(vec2 s, vec2 e, vec2 p)
{
	vec2 m = p - s;
	vec2 n = e - s;
	
	float len = length(n);
	n /= len;
	
	vec2 proj = clamp(dot(m, n), 0., len) * n;
	
	return length(m - proj);
}


void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
	vec2 sp = vec2(0.5, 0.5);
	vec2 ep = vec2(1., 1.);
	float lineWidth = 5.;
	vec4 color = vec4(1.);

	vec2 uv = fragCoord.xy / iResolution.xy;
	float d = lineDistance(sp, ep, fragCoord.xy);
	fragColor = vec4(color.rgb, color.a * clamp(lineWidth / 2. - d, 0., 1.));
}
