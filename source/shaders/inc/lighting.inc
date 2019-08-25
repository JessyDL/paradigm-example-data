#include "math.inc"

// region @projection
float textureProj(sampler2DArray samp, vec4 P, float layer, vec2 offset)
{
	float shadow = 1.0;
	vec4 shadowCoord = P / P.w;
	shadowCoord.st = shadowCoord.st * 0.5 + 0.5;
	
	if (shadowCoord.z > -1.0 && shadowCoord.z < 1.0) 
	{
		float dist = texture(samp, vec3(shadowCoord.st + offset, layer)).r;
		if (shadowCoord.w > 0.0 && dist < shadowCoord.z) 
		{
			shadow = 0.0f;
		}
	}
	return shadow;
}

float textureProj(sampler2D samp, vec4 P, vec2 offset)
{
	float shadow = 1.0;
	vec4 shadowCoord = P / P.w;
	shadowCoord.st = shadowCoord.st * 0.5 + 0.5;
	
	if (shadowCoord.z > -1.0 && shadowCoord.z < 1.0) 
	{
		float dist = texture(samp, vec2(shadowCoord.st + offset)).r;
		if (shadowCoord.w > 0.0 && dist < shadowCoord.z) 
		{
			shadow = 0.0f;
		}
	}
	return shadow;
}

// region @filtering
float filterPCF(sampler2DArray samp, vec4 sc, float layer)
{
	ivec2 texDim = textureSize(samp, 0).xy;
	float scale = 0.125;
	float dx = scale * 1.0 / float(texDim.x);
	float dy = scale * 1.0 / float(texDim.y);

	float shadowFactor = 0.0;
	int count = 0;
	int range = 1;
	
	for (int x = -range; x <= range; x++)
	{
		for (int y = -range; y <= range; y++)
		{
			shadowFactor += textureProj(samp, sc, layer, vec2(dx*x, dy*y));
			count++;
		}
	
	}
	return shadowFactor / count;
}


float filterPCF(sampler2D samp, vec4 sc)
{
	ivec2 texDim = textureSize(samp, 0).xy;
	float scale = 0.125;
	float dx = scale * 1.0 / float(texDim.x);
	float dy = scale * 1.0 / float(texDim.y);

	float shadowFactor = 0.0;
	int count = 0;
	int range = 1;
	
	for (int x = -range; x <= range; x++)
	{
		for (int y = -range; y <= range; y++)
		{
			shadowFactor += textureProj(samp, sc, vec2(dx*x, dy*y));
			count++;
		}
	
	}
	return shadowFactor / count;
}
// endregion
// endregion

// ================================================================================================
// Helpers
// ================================================================================================
// region @Helpers
vec3 Fresnel(vec3 reflectance, float x)
{
    return reflectance + (convertFloatVec3(1.0) - reflectance - convertFloatVec3(2.0) * reflectance * convertFloatVec3(x)) * convertFloatVec3(pow(1.0 - x, 5.0));
}

vec3 FresnelSchlick(vec3 specAlbedo, vec3 h, vec3 l)
{
    float lDotH = saturate(dot(l, h));
    vec3 fresnel = specAlbedo + (convertFloatVec3(1.0f) - specAlbedo) * convertFloatVec3(pow((1.0f - lDotH), 5.0f));

    // Disable specular entirely if the albedo is set to 0.0
	if(dot(specAlbedo, convertFloatVec3(1.0f)) <= 0.0f)
		fresnel = vec3(0.0f);
	
    return fresnel;
}
 
float DiffuseConservation()
{
    return 1.0 / 3.14159265f;
}
 
float SpecularConservation(float power)
{
    return 0.0397436 * power + 0.0856832;
}

float3 SampleEnv(float3 direction)
{    
    if(direction.y >= 0.0f)
        return lerp(float3(0.2f, 0.2f, 0.2f), float3(0.0f, 0.2f,  0.4f), direction.y);
    
    return lerp(float3(0.2f, 0.2f, 0.2f), float3(0.01f, 0.1f, 0.01f), -direction.y);
}

float Luminance(float3 color)
{
	float3 rgb2lum = float3(0.30f, 0.59f, 0.11f);
	return dot(color, rgb2lum);
}

// Helper for computing the Beckmann geometry term
float Beckmann_G1(float m, float nDotX)
{
    float nDotX2 = nDotX * nDotX;
    float tanTheta = sqrt((1 - nDotX2) / nDotX2);
    float a = 1.0f / (m * tanTheta);
    float a2 = a * a;

    float g = 1.0f;
    if(a < 1.6f)
        g *= (3.535f * a + 2.181f * a2) / (1.0f + 2.276f * a + 2.577f * a2);

    return g;
}

float GGX_V1(float m2, float nDotX)
{
    return 1.0f / (nDotX + sqrt(m2 + (1 - m2) * nDotX * nDotX));
}
// endregion

// ================================================================================================
// Computes the specular contribution of a light using normalized Blinn-Phong
// ================================================================================================
float CalculateSpecular(float3 normal, float3 halfVector, float roughness) {
    // Convert roughness to a specular power
    float specPower = pow(2.0f, saturate(1.0f - roughness) * 12.0f);

    float nDotH = saturate(dot(normal, halfVector));
    return pow(nDotH, specPower) * ((specPower + 8.0f) / (8.0f * 3.14159265f));
}

// ================================================================================================
// Calculate the Fresnel factor using Schlick's approximation
// ================================================================================================
float3 CalculateFresnel(float3 specAlbedo, float3 halfVector, float3 lightDir) 
{
    float3 fresnel = specAlbedo;
    fresnel += (convertFloatVec3(1.0f) - specAlbedo) * convertFloatVec3(pow((1.0f - saturate(dot(halfVector, lightDir))), 5.0f));
    return saturate3(fresnel);
}
// ===============================================================================================
// Computes the specular term using a Beckmann microfacet distribution, with a matching
// geometry factor and visibility term. Based on "Microfacet Models for Refraction Through
// Rough Surfaces" [Walter 07]. m is roughness, n is the surface normal, h is the half vector,
// l is the direction to the light source, and specAlbedo is the RGB specular albedo
// ===============================================================================================
float Beckmann_Specular(float m, float3 n, float3 h, float3 v, float3 l)
{
    float nDotH = max(dot(n, h), 0.0001f);
    float nDotL = saturate(dot(n, l));
    float nDotV = max(dot(n, v), 0.0001f);

    float nDotH2 = nDotH * nDotH;
    float nDotH4 = nDotH2 * nDotH2;
    float m2 = m * m;

    // Calculate the distribution term
    float tanTheta2 = (1 - nDotH2) / nDotH2;
    float expTerm = exp(-tanTheta2 / m2);
    float d = expTerm / (3.14159265f * m2 * nDotH4);

    // Calculate the matching geometric term
    float g1i = Beckmann_G1(m, nDotL);
    float g1o = Beckmann_G1(m, nDotV);
    float g = g1i * g1o;

    return d * g * (1.0f / (4.0f * nDotL * nDotV));
}

// ===============================================================================================
// Computes the specular term using a GGX microfacet distribution, with a matching
// geometry factor and visibility term. Based on "Microfacet Models for Refraction Through
// Rough Surfaces" [Walter 07]. m is roughness, n is the surface normal, h is the half vector,
// l is the direction to the light source, and specAlbedo is the RGB specular albedo
// ===============================================================================================
float GGX_Specular(float m, float3 n, float3 h, float3 v, float3 l)
{
    float nDotH = saturate(dot(n, h));
    float nDotL = saturate(dot(n, l));
    float nDotV = saturate(dot(n, v));

    float nDotH2 = nDotH * nDotH;
    float m2 = m * m;

    // Calculate the distribution term
    float d = m2 / (3.14159265f * pow(nDotH * nDotH * (m2 - 1) + 1, 2.0f));

    // Calculate the matching visibility term
    float v1i = GGX_V1(m2, nDotL);
    float v1o = GGX_V1(m2, nDotV);
    float vis = v1i * v1o;

    return d * vis;
}

// ================================================================================================
// Calculate an approximate Fresnel factor using N dot V instead of L dot H
// ================================================================================================
vec3 CalculateAmbientFresnel(float3 specAlbedo, float3 normal, float3 view, float roughness)
{
	float fSmooth = 1.0f - roughness;
	float powerterm = pow((1.0f - saturate(dot(normal, view))), 5.0f);
	float3 fresnel = specAlbedo + (max(convertFloatVec3(fSmooth), specAlbedo) - specAlbedo) * convertFloatVec3(powerterm);
	return saturate3(fresnel);
}
// ================================================================================================
// Calculate the lighting contribution given a light direction and material properties
// ================================================================================================
float3 CalcLighting(float3 normal, float3 lightDir, float3 lightColor,
					float3 diffuseAlbedo, float3 specularAlbedo, float roughness,
					float3 surfacePos, float3 cameraPos)
{
	float3 lighting = convertFloatVec3(0.0f);
    float nDotL = saturate(dot(normal, -lightDir));
	float3 fresnel = convertFloatVec3(0.0f);
	float specular = 0.0f;
    if(nDotL > 0.0f)
    {
        float3 view = normalize(cameraPos - surfacePos);
        float3 h = normalize(view - lightDir);

        //specular = Beckmann_Specular(roughness, normal, h, view, -lightDir);
		specular = GGX_Specular(roughness, normal, h, view, -lightDir);
        fresnel = FresnelSchlick(specularAlbedo, h, -lightDir);

        lighting = (diffuseAlbedo * DiffuseConservation() + specular * fresnel * diffuseAlbedo * 2) * nDotL * lightColor * 3.14159265f;
    }

    return max(lighting, 0.0f);
	
	//return color;
}

// ================================================================================================
// Calculate the lighting contribution for a single point light
// ================================================================================================
float3 CalcPointLight(in float3 normal, in float3 lightColor, in float lightRadius, in float3 diffuseAlbedo,
					  in float3 specularAlbedo, in float roughness, in float3 surfacePos,
					  in float3 lightPos, in float3 cameraPos)
{
	float3 L = lightPos - surfacePos;
	float distance = length(L);
	float3 lightDir = L / distance;
	
	float d = max(distance - lightRadius, 0);
    L /= distance;
	
	float denom = d/lightRadius + 1;
    float attenuation = 1 / (denom*denom);
	const float cutoff = 0.005f;
	attenuation = (attenuation - cutoff) / (1 - cutoff);
    attenuation = max(attenuation, 0);
	
	attenuation = clamp(1.0 - distance*distance/(lightRadius*lightRadius), 0.0, 1.0); 
	attenuation *= attenuation;
	
    return CalcLighting(normal, -lightDir, lightColor, diffuseAlbedo, specularAlbedo,
						roughness, surfacePos, cameraPos) * attenuation;
}

float2 LightingFuncGGX_FV(float dotLH, float roughness)
{
	float alpha = roughness*roughness;

	// F
	float F_a, F_b;
	float dotLH5 = pow(1.0f-dotLH,5);
	F_a = 1.0f;
	F_b = dotLH5;

	// V
	float vis;
	float k = alpha/2.0f;
	float k2 = k*k;
	float invK2 = 1.0f-k2;
	vis = inversesqrt(dotLH*dotLH*invK2 + k2);

	return float2(F_a*vis,F_b*vis);
}

float LightingFuncGGX_D(float dotNH, float roughness)
{
	float alpha = roughness*roughness;
	float alphaSqr = alpha*alpha;
	float pi = 3.14159f;
	float denom = dotNH * dotNH *(alphaSqr-1.0) + 1.0f;

	float D = alphaSqr/(pi * denom * denom);
	return D;
}

float LightingFuncGGX_OPT3(float3 N, float3 V, float3 L, float roughness, float F0)
{
	float3 H = normalize(V+L);

	float dotNL = saturate(dot(N,L));
	float dotLH = saturate(dot(L,H));
	float dotNH = saturate(dot(N,H));

	float D = LightingFuncGGX_D(dotNH,roughness);
	float2 FV_helper = LightingFuncGGX_FV(dotLH,roughness);
	float FV = F0*FV_helper.x + (1.0f-F0)*FV_helper.y;
	float specular = dotNL * D * FV;

	return specular;
}

// phong (lambertian) diffuse term
float phong_diffuse()
{
    return (1.0 / PI);
}


// compute fresnel specular factor for given base specular and product
// product could be NdV or VdH depending on used technique
vec3 fresnel_factor(in vec3 f0, in float product)
{
	return Fresnel(f0, product);
    return mix(f0, vec3(1.0), pow(1.01 - product, 5.0));
}

// following functions are copies of UE4
// for computing cook-torrance specular lighting terms
float D_blinn(in float roughness, in float NdH)
{
    float m = roughness * roughness;
    float m2 = m * m;
    float n = 2.0 / m2 - 2.0;
    return (n + 2.0) / (2.0 * PI) * pow(NdH, n);
}


float D_beckmann(in float roughness, in float NdH)
{
    float m = roughness * roughness;
    float m2 = m * m;
    float NdH2 = NdH * NdH;
    return exp((NdH2 - 1.0) / (m2 * NdH2)) / (PI * m2 * NdH2 * NdH2);
}

float D_GGX(in float roughness, in float NdH)
{
    float m = roughness * roughness;
    float m2 = m * m;
    float d = (NdH * m2 - NdH) * NdH + 1.0;
    return m2 / (PI * d * d);
}

float G_schlick(in float roughness, in float NdV, in float NdL)
{
    float k = roughness * roughness * 0.5;
    float V = NdV * (1.0 - k) + k;
    float L = NdL * (1.0 - k) + k;
    return 0.25 / (V * L);
}


// simple phong specular calculation with normalization
vec3 phong_specular(in vec3 V, in vec3 L, in vec3 N, in vec3 specular, in float roughness)
{
    vec3 R = reflect(-L, N);
    float spec = max(0.0, dot(V, R));

    float k = 1.999 / (roughness * roughness);

    return min(1.0, 3.0 * 0.0398 * k) * pow(spec, min(10000.0, k)) * specular;
}

// simple blinn specular calculation with normalization
vec3 blinn_specular(in float NdH, in vec3 specular, in float roughness)
{
    float k = 1.999 / (roughness * roughness);
    
    return min(1.0, 3.0 * 0.0398 * k) * pow(NdH, min(10000.0, k)) * specular;
}


#define PHONG_DIFFUSE
#define BLINN_SPECULAR

// cook-torrance specular calculation                      
vec3 cooktorrance_specular(in float NdL, in float NdV, in float NdH, in vec3 specular, in float roughness, in float rimAmount)
{
#if defined(COOK_BECKMANN)
    float D = D_beckmann(roughness, NdH);
#elif defined(COOK_GGX)
    float D = D_GGX(roughness, NdH);
#elif defined (BLINN_SPECULAR)
    float D = D_blinn(roughness, NdH);
#endif

    float G = G_schlick(roughness, NdV, NdL);

    float rim = mix(1.0 - roughness * rimAmount * 0.9, 1.0, NdV);

    return (1.0 / rim) * specular * G * D;
}

void Lighting(float3 normal, float3 lightDir, float3 lightColor,
					float3 diffuseAlbedo, float3 specularAlbedo, float roughness,
					float3 surfacePos, float3 cameraPos, out vec3 diffuse_reflection, out vec3 specular_reflection)
{	
	vec3 N = normal;
	vec3 L = -lightDir;
	vec3 V = normalize(cameraPos - surfacePos);
	vec3 H = normalize(V - L);
	
    float NdL = saturate(dot(N, L));
    float NdV = saturate(dot(N, V));
    float NdH = saturate(dot(N, H));
    float HdV = saturate(dot(H, V));
    float LdV = saturate(dot(L, V));
	
#if defined (BLINN_DIFFUSE)
    // specular reflectance with BLINN
    vec3 specfresnel = fresnel_factor(specularAlbedo, HdV);
    vec3 specref = blinn_specular(NdH, specfresnel, roughness);
#elif defined (COOK_DIFFUSE)
    // specular reflectance with COOK-TORRANCE
    vec3 specfresnel = fresnel_factor(specularAlbedo, HdV);
    vec3 specref = cooktorrance_specular(NdL, NdV, NdH, specfresnel, roughness, 1);
#elif defined (PHONG_DIFFUSE)
	vec3 specfresnel = fresnel_factor(specularAlbedo, NdV);
    vec3 specref = phong_specular(V, L, N, specfresnel, roughness);
#endif
    specular_reflection = specref * vec3(NdL) * PI;
	
    // diffuse is common for any model
    diffuse_reflection = (vec3(1.0) - specfresnel * (1-roughness)) * phong_diffuse() * NdL;
    //diffuse_reflection = (vec3(1.0) - specfresnel) * phong_diffuse() * NdL;
}

void LightingPoint(in float3 normal, in float3 lightColor, in float lightRadius, in float3 diffuseAlbedo,
					  in float3 specularAlbedo, in float roughness, in float3 surfacePos,
					  in float3 lightPos, in float3 cameraPos, out vec3 diffuse_reflection, out vec3 specular_reflection)
{
	float3 L = lightPos - surfacePos;
	float distance = length(L);
	float3 lightDir = L / distance;
	
	float d = max(distance - lightRadius, 0);
    L /= distance;
	
	float denom = d/lightRadius + 1;
    float attenuation = 1 / (denom*denom);
	const float cutoff = 0.005f;
	attenuation = (attenuation - cutoff) / (1 - cutoff);
    attenuation = max(attenuation, 0);
	
	attenuation = clamp(1.0 - distance*distance/(lightRadius*lightRadius), 0.0, 1.0); 
	attenuation *= attenuation;
	
    Lighting(normal, -lightDir, lightColor, diffuseAlbedo, specularAlbedo, roughness, surfacePos, cameraPos, diffuse_reflection, specular_reflection);
	diffuse_reflection*= attenuation;
	specular_reflection*= attenuation;
}