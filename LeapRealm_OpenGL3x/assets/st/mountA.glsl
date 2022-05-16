
// math
#define time iTime
#define PI 3.141592653589
#define saturate(x) clamp(x,0.,1.)
float hash( vec3 x );
float hash( vec2 p );
float hash( float p );
float hash2Interleaved( vec2 x );
float noise( vec3 x );
vec4 noised( vec3 x );
mat2 rotate( float t );

// mapping
vec3 randomSphereDir( vec2 rnd );
vec3 randomHemisphereDir( vec3 dir, float i );
vec4 tex3D( sampler2D tex, vec3 p, vec3 n );
vec3 bumpMapping( sampler2D tex, vec3 p, vec3 n, float bf );

// tone mapping
vec3 acesToneMapping( vec3 col );
vec3 filmicToneMapping( vec3 col );





// ---------------------------------------------
// Math
// ---------------------------------------------
float hash( vec3 p )
{
    return fract(sin(dot(p,vec3(127.1,311.7, 74.7)))*43758.5453123);
}

float hash( vec2 p )
{
    return fract(sin(dot(p,vec2(127.1,311.7)))*43758.5453123);
}

float hash( float p ) 
{
    return fract(sin(p)*43758.5453123);
}

float hash2Interleaved( vec2 x )
{
    // between random & dithered pattern
    // good for jittering and blur as well as blue noise :)
    // http://www.iryoku.com/next-generation-post-processing-in-call-of-duty-advanced-warfare
    vec3 magic = vec3( 0.06711056, 0.00583715, 52.9829189 );
    return fract( magic.z * fract( dot( x, magic.xy ) ) );
}

vec4 noised( vec3 x )
{
	// https://iquilezles.org/articles/gradientnoise
    vec3 p = floor(x);
    vec3 w = fract(x);
    
    vec3 u = w*w*w*(w*(w*6.0-15.0)+10.0);
    vec3 du = 30.0*w*w*(w*(w-2.0)+1.0);

    float a = hash( p+vec3(0,0,0) );
    float b = hash( p+vec3(1,0,0) );
    float c = hash( p+vec3(0,1,0) );
    float d = hash( p+vec3(1,1,0) );
    float e = hash( p+vec3(0,0,1) );
    float f = hash( p+vec3(1,0,1) );
    float g = hash( p+vec3(0,1,1) );
    float h = hash( p+vec3(1,1,1) );

    float k0 =   a;
    float k1 =   b - a;
    float k2 =   c - a;
    float k3 =   e - a;
    float k4 =   a - b - c + d;
    float k5 =   a - c - e + g;
    float k6 =   a - b - e + f;
    float k7 = - a + b + c - d + e - f - g + h;

    return vec4( -1.0+2.0*(k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z), 
                      2.0* du * vec3( k1 + k4*u.y + k6*u.z + k7*u.y*u.z,
                                      k2 + k5*u.z + k4*u.x + k7*u.z*u.x,
                                      k3 + k6*u.x + k5*u.y + k7*u.x*u.y ) ).yzwx;
}
float noise( vec3 x )
{
	// https://iquilezles.org/articles/gradientnoise
    vec3 p = floor(x);
    vec3 w = fract(x);
    
    vec3 u = w*w*w*(w*(w*6.0-15.0)+10.0);
    vec3 du = 30.0*w*w*(w*(w-2.0)+1.0);

    float a = hash( p+vec3(0,0,0) );
    float b = hash( p+vec3(1,0,0) );
    float c = hash( p+vec3(0,1,0) );
    float d = hash( p+vec3(1,1,0) );
    float e = hash( p+vec3(0,0,1) );
    float f = hash( p+vec3(1,0,1) );
    float g = hash( p+vec3(0,1,1) );
    float h = hash( p+vec3(1,1,1) );

    float k0 =   a;
    float k1 =   b - a;
    float k2 =   c - a;
    float k3 =   e - a;
    float k4 =   a - b - c + d;
    float k5 =   a - c - e + g;
    float k6 =   a - b - e + f;
    float k7 = - a + b + c - d + e - f - g + h;
    return -1.0+2.0*(k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z);
}

mat2 rotate( float t ) {
    float a = cos(t);
    float b = sin(t);
    
    return mat2( a, b, -b, a );
}


// ---------------------------------------------
// Mapping
// ---------------------------------------------
vec3 randomSphereDir( vec2 rnd )
{
    float s = rnd.x*PI*2.;
    float t = rnd.y*2.-1.;
    return vec3(sin(s), cos(s), t) / sqrt(1.0 + t * t);
}

vec3 randomHemisphereDir( vec3 dir, float i )
{
    vec3 v = randomSphereDir( vec2(hash(i+1.), hash(i+2.)) );
    return v * sign(dot(v, dir));
}

vec4 tex3D( sampler2D tex, vec3 p, vec3 n )
{
    n = abs(n);
    
    vec4 c = texture(tex, p.yz) * n.x;
    c += texture(tex, p.xz) * n.y;
    c += texture(tex, p.xy) * n.z;
    
    return c / 3.;
}

vec3 bumpMapping( sampler2D tex, vec3 p, vec3 n, float bf )
{
    // clever code taken from Shane
    // https://www.shadertoy.com/view/MscSDB
    const vec2 e = vec2(0.001, 0);
    
    mat3 m = mat3( tex3D(tex, p - e.xyy, n).rgb,
                   tex3D(tex, p - e.yxy, n).rgb,
                   tex3D(tex, p - e.yyx, n).rgb);
    
    vec3 g = vec3(0.299, 0.587, 0.114) * m;
    g = (g - dot( tex3D(tex,  p , n).rgb, vec3(0.299, 0.587, 0.114)) )/e.x;
    g -= n * dot(n, g);
                      
    return normalize( n + g*bf );
    
}

// ---------------------------------------------
// Tone mapping
// ---------------------------------------------
vec3 acesToneMapping( vec3 col )
{
    // https://www.shadertoy.com/view/XlKSDR
    // Narkowicz 2015, "ACES Filmic Tone Mapping Curve"
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return (col * (a * col + b)) / (col * (c * col + d) + e);
}

vec3 filmicToneMapping( vec3 col )
{
    // Good reference
    // https://www.shadertoy.com/view/lslGzl
    col = max(vec3(0.), col - vec3(0.004));
    col = (col * (6.2 * col + .5)) / (col * (6.2 * col + 1.7) + 0.06);
    return col;
}

// ---------------------------------------------
// Main Rendering
// ---------------------------------------------
#define ENABLE_CLOUDS
#define ENABLE_CLOUDS_SHADOWING
//#define ENABLE_TEMPORAL_JITTERING


// raymarching
vec3 raymarch( vec3 ro, vec3 rd, const vec2 nf, const float eps );
vec3 normal( vec3 p, const float eps );
float ambientOcclusion( vec3 p, vec3 n, float maxDist, float falloff );
float shadow( vec3 ro, vec3 rd, float mint, float tmax );
float shadowFast( vec3 ro, vec3 rd, float mint, float tmax );


vec3 sunDir;



// ---------------------------------------------
// SDF
// ---------------------------------------------
float displacement( vec3 p )
{
    // more cool tricks -> https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch01.html
    vec3 pp = p;
    float mgn = .5;
    float d = 0.;
    float s = 1.;
    for(int i=0; i<5; i++)
    {
        vec4 rnd = noised(p+10.);
        d += rnd.w * mgn;
        
        p *= 2.;
        p += rnd.xyz*.2*s;
        if(i==2)
            s *= -1.;
        mgn *= .5;
        
    }
    
    p = pp*pow(2.,5.);
    for(int i=0; i<4; i++)
    {
        vec4 rnd = noised(p);
        d += rnd.w * mgn;
        
        p *= 2.;
        mgn *= .5;
        
    }
    
    
    return d;
}


float clouds( vec3 p )
{
    vec3 pp = p;
    p.y -= time*.1;
    p *= .23;
    float mgn = .5;
    float d = 0.;
    for(int i=0; i<5; i++)
    {
        float rnd = noise(p);
        d += rnd * mgn;
        
        p *= 2.;
        mgn *= .6;
        
    }
    return -(pp.y*.2+1.) + d*.5+.5;
}


float map( vec3 p )
{
    float d = p.y*.5 + (displacement(p*.1))*10.3;
    
    return d*.4;
}



// ---------------------------------------------
// Shading
// ---------------------------------------------
vec3 skyColor( vec3 rd )
{
    // Heavely inspired by https://www.shadertoy.com/view/Ml2cWG
    rd.y = max(rd.y+0.03, 0.02);
	const float anisotropicIntensity = 0.03; 
	const float density = .3;
    
    
    float l = length(rd - sunDir);
    
    vec3 col = vec3(0.39, 0.57, 1.0) * (1.0 + anisotropicIntensity);
    
	float zenith = density / pow(max(rd.y, 0.35e-2), 0.75);
    vec3 absorption = exp2(col * -zenith) * 2.;
	float rayleig = 1.0 + pow(1.0 - clamp(l, 0.0, 1.0), 2.0) * PI * 0.5;
    
    vec3 sun = vec3(1.,.5,.1) * smoothstep(0.03, 0.0, l) * 100.0;
    
    return col * zenith * rayleig * absorption*.5 + sun;
}
vec3 clouds( vec3 col, vec3 ro, vec3 rd, float l )
{
    for(int i=0; i<32; i++)
    {
        vec3 p = ro + rd * (1. - float(i)/64.) * l;
        float d = clouds(p);
        
        #ifdef ENABLE_CLOUDS_SHADOWING
        float shad = 1.;
        if(d > .01)
            shad = shadowFast(p, sunDir, .4, 30.);
        #else
        float shad = 1.;
        #endif
        vec3 c = vec3(.4)*exp(-d*10.) + vec3(1.,.7,.4)*3. *shad*pow(saturate(d-clouds(p+sunDir)*2.),1.);
        
        col = mix(col, c, saturate(d*.7));
    }
    return col;
}
vec3 shade( vec3 p, vec3 n, vec3 ro, vec3 rd )
{
    float d = length(p-ro);
    vec3 sky = skyColor(rd);
    vec3 col = sky;
    
    if (d < 80.)
    {
        vec3 albedo = mix(vec3(1.,.6,.34)*.1, vec3(.7)*.1, pow(saturate(n.y),4.)+(noised(p*vec3(.1,1.,1.)).w*.3));
        
		n = bumpMapping(iChannel0, p*1.5, n, 1.);


        float ao = ambientOcclusion(p,n, 5.1, 2.) * ambientOcclusion(p,n, 1., 1.5);
        float shad = shadow(vec3(p), vec3(sunDir), float(0.4), float(30.));

        vec3 amb = vec3(.3,.4,.5) * ao;
        vec3 diff = vec3(1.,.9,.7) * max(0., dot(n, sunDir) ) * shad;
        vec3 bnc = vec3(1.,.7,.4) * saturate(-n.y) * ao;
        
        col = albedo * ( diff*10. + amb*10. + bnc*3. );

        //return vec3(ao);
        #ifdef ENABLE_CLOUDS
    	col = clouds(col,ro,rd,d);
        #endif
        col = mix(col, vec3(.6,.7,1.)*.5, 1. - exp(-0.005*d) );
        
    }
    
    
    return col;
}



// ---------------------------------------------
// SDF
// ---------------------------------------------
void mainImage( out vec4 fragColor, vec2 fragCoord )
{
    vec2 invRes = vec2(1.) / iResolution.xy;
    vec2 uv = fragCoord * invRes;
    
    vec2 jitt = vec2(0.);
    #ifdef ENABLE_TEMPORAL_JITTERING
    jitt = vec2(hash(iTime)-.5, hash(iTime+1.)-.5) * invRes;
	#endif
    vec2 v = -1.0+2.0*(uv+jitt);
	v.x *= iResolution.x/iResolution.y;
    
    
	sunDir = normalize( vec3(cos(time*.02+1.)+1., 1., .4) );
    
    // camera ray
    vec3 ro = vec3(-1., 4.5, 182.);
    vec3 rd = normalize( vec3(v, 1.45) );
    rd.yz = rotate(.15) * rd.yz;
    rd.xz = rotate(.1) * rd.xz;
    
    // classic sdf rendering
    vec3 p = raymarch(ro, rd, vec2(5.,80.), 0.001);
    vec3 n = normal(p, 0.001);
    vec3 col = shade(p, n, ro, rd);
    
    
    
    float depth = length(ro-p)/100.;
    float coc = saturate(pow(depth+.65,50.));
    fragColor = vec4(col,coc);
    
    // temporal jittering
    #ifdef ENABLE_TEMPORAL_JITTERING
    fragColor = mix(fragColor, texture(iChannel1,uv), vec4(.8));
    #endif
}






// ---------------------------------------------
// Raymarching
// Can't be to Common because of the map() dependency :(
// ---------------------------------------------
vec3 raymarch( vec3 ro, vec3 rd, const vec2 nf, const float eps ) {
    vec3 p = ro + rd * nf.x;
    float l = 0.;
    for(int i=0; i<128; i++) {
		float d = map(p);
        l += d;
        p += rd * d;
        
        if(d < eps || l > nf.y)
            break;
    }
    
    return p;
}

vec3 normal( vec3 p, const float eps ) {
    float d = map(p);
    vec2 e = vec2(eps, 0.);
    
    vec3 n;
    
    n.x = d - map(p-e.xyy);
    n.y = d - map(p-e.yxy);
    n.z = d - map(p-e.yyx);
    
    return normalize(n);
}

float ambientOcclusion( vec3 p, vec3 n, float maxDist, float falloff )
{
	const int nbIte = 8;
    const float nbIteInv = 1./float(nbIte);
    const float rad = 1.-1.*nbIteInv; //Hemispherical factor (self occlusion correction)
    
	float ao = 0.0;
    
    for( int i=0; i<nbIte; i++ )
    {
        float l = hash(float(i))*maxDist;
        vec3 rd = normalize(n+randomHemisphereDir(n, l )*rad)*l; // mix direction with the normal
        													    // for self occlusion problems!
        
        ao += (l - max(map( p + rd ),0.)) / maxDist * falloff;
    }
	
    return clamp( 1.-ao*nbIteInv, 0., 1.);
}

// https://www.shadertoy.com/view/lsKcDD
float shadow( vec3 ro, vec3 rd, float mint, float tmax )
{
	float res = 1.0;
    float t = mint;
    
    for( int i=0; i<32; i++ )
    {
		float h = map( ro + rd*t );
		res = min( res, 10.0*h/t );
        t += h;
        
        if( res<0.0001 || t>tmax ) break;
        
    }
    return clamp( res, 0.0, 1.0 );
}
float shadowFast( vec3 ro, vec3 rd, float mint, float tmax )
{
	float res = 1.0;
    float t = mint;
    
    for( int i=0; i<16; i++ )
    {
		float h = map( ro + rd*t )*2.;
		res = min( res, 10.0*h/t );
        t += h;
        
        if( res<0.0001 || t>tmax ) break;
        
    }
    return clamp( res, 0.0, 1.0 );
}
