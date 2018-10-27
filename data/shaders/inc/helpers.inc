#include "math.inc"

vec3 decodeTangentSpace (vec2 enc)
{
    vec3 n;
	n.xy = enc;
	n.x = -n.x;
	n.z = sqrt(1.0 - dot(n.xy, n.xy));
	//n.z = sqrt(1 - n.x*n.x - n.y*n.y);
	n.y = -n.y;
    return normalize(n);
}

vec2 encodeWSNormals (vec3 n)
{
    return vec2(
      (vec2(atan(n.y, n.x)/PI, n.z)+1.0)*0.5);
}

vec3 decodeWSNormals (vec2 enc)
{
    vec2 ang = enc*2-1;
    vec2 scth;
    scth.x = sin(ang.x * PI);
    scth.y = cos(ang.x * PI);
    vec2 scphi = vec2(sqrt(1.0 - ang.y*ang.y), ang.y);
    return normalize(vec3(scth.y*scphi.x, scth.x*scphi.x, scphi.y));
}

float LinearizeDepth(sampler2D depthTexture, vec2 uv, float near, float far)
{
  float z = texture(depthTexture, uv).x;
  float C = 2.0 / log2(far + near);
  z = (pow(C*far+near,z)-near)/C;
  return z/far;
}

// Applies the filmic curve from John Hable's presentation
vec3 ToneMapFilmicALU(vec3 color)
{
	const float A = 0.15f;
	const float B = 0.50f;
	const float C = 0.10f;
	const float D = 0.20f;
	const float E = 0.02f;
	const float F = 0.30f;
	const float W = 11.2f;

	float ExposureBias = 2.0;
	vec3 x = ExposureBias*color;
	vec3 curr = ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;

	x = vec3(W, W, W);
	vec3 whiteScale = 1.0f/(((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F);
	color = color*whiteScale;
	//color = pow3(abs3(color), 2.2f);
	return color;
}


vec3 applyFog( in vec3  rgb,      // original color of the pixel
               in float distance, // camera to point distance
               in float  sunSS,   // camera to point vector
			   in vec3 fogColor,
			   in vec3 sunColor)
{
    float sunAmount = 1-saturate( 1-((1-sunSS)*0.5f));
    //sunAmount = 1-saturate( sunSS);
    vec3  fc  = mix( fogColor, // bluish
                           sunColor,
                           saturate(pow(sunAmount,16.0)) );
	vec3 be = vec3(1,1,1);
	vec3 bi = vec3(1,1,1);
	vec3 extColor = vec3( exp(-distance*be.x), exp(-distance*be.y), exp(-distance*be.z) );
	vec3 insColor = vec3( exp(-distance*bi.x), exp(-distance*bi.y), exp(-distance*bi.z) );
	return rgb*(1.0-extColor) + fc *insColor;
}

vec3 applyFog( in vec3  rgb,      // original color of the pixel
               in float distance, // camera to point distance
               in vec3  rayDir,   // camera to point vector
               in vec3  sunDir,  // sun light direction
			   in vec3 fogColor,
			   in vec3 sunColor )
{
	return applyFog(rgb, distance, dot( normalize(rayDir), normalize(sunDir) ), fogColor, sunColor);
}


//http://www.thetenthplanet.de/archives/1180
mat3 cotangent_frame( vec3 N, vec3 p, vec2 uv )
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
 
    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
    // construct a scale-invariant frame 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}

vec3 DecodeNormalMap( sampler2D normalMap, vec2 texCoord, vec3 vertexNormal, vec3 view )
{
    // assume N, the interpolated vertex normal and 
    // V, the view vector (vertex to eye)
    vec4 map = texture(normalMap, texCoord);
	map.xyz = decodeTangentSpace(map.ag * 2 - 1);
	//vertexNormal.xy = -vertexNormal.xy;
    mat3 TBN = cotangent_frame( vertexNormal, -view, texCoord );
    return normalize( TBN * map.xyz );
}