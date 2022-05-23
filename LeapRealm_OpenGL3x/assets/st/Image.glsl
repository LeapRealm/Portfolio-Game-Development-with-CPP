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
    uniform float	  iActionKeys[5];
	uniform float	  iAxisKeys[5];
#endif

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = (-iResolution.xy + 2.0*fragCoord) / iResolution.y;

    vec3 col = vec3(0.0);

    // state
    col = mix( col, vec3(1.0,0.0,0.0), 
        (1.0-smoothstep(0.3,0.31,length(uv-vec2(-0.75,0.0))))*(0.3+0.7*iAxisKeys[0]));

    col = mix( col, vec3(1.0,1.0,0.0), 
        (1.0-smoothstep(0.3,0.31,length(uv-vec2(0.0,0.5))))*(0.3+0.7*iAxisKeys[1]));
	
    col = mix( col, vec3(0.0,1.0,0.0),
        (1.0-smoothstep(0.3,0.31,length(uv-vec2(0.75,0.0))))*(0.3+0.7*iAxisKeys[2]));

    col = mix( col, vec3(0.0,0.0,1.0),
        (1.0-smoothstep(0.3,0.31,length(uv-vec2(0.0,-0.5))))*(0.3+0.7*iAxisKeys[3]));

    // keypress	
    col = mix( col, vec3(1.0,0.0,0.0), 
        (1.0-smoothstep(0.0,0.01,abs(length(uv-vec2(-0.75,0.0))-0.35)))*iActionKeys[0]);
	
    col = mix( col, vec3(1.0,1.0,0.0),
        (1.0-smoothstep(0.0,0.01,abs(length(uv-vec2(0.0,0.5))-0.35)))*iActionKeys[1]);

    col = mix( col, vec3(0.0,1.0,0.0),
        (1.0-smoothstep(0.0,0.01,abs(length(uv-vec2(0.75,0.0))-0.35)))*iActionKeys[2]);
	
    col = mix( col, vec3(0.0,0.0,1.0),
        (1.0-smoothstep(0.0,0.01,abs(length(uv-vec2(0.0,-0.5))-0.35)))*iActionKeys[3]);
    
    // toggle	
    col = mix( col, vec3(1.0,0.0,0.0), 
        (1.0-smoothstep(0.0,0.01,abs(length(uv-vec2(-0.75,0.0))-0.3)))*1.);
	
    col = mix( col, vec3(1.0,1.0,0.0),
        (1.0-smoothstep(0.0,0.01,abs(length(uv-vec2(0.0,0.5))-0.3)))*1.);

    col = mix( col, vec3(0.0,1.0,0.0),
        (1.0-smoothstep(0.0,0.01,abs(length(uv-vec2(0.75,0.0))-0.3)))*1.);
	
    col = mix( col, vec3(0.0,0.0,1.0),
        (1.0-smoothstep(0.0,0.01,abs(length(uv-vec2(0.0,-0.5))-0.3)))*1.);

    fragColor = vec4(col,1.0);
}