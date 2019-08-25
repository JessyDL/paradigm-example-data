#define float3 vec3
#define float4 vec4
#define float2 vec2
#define lerp mix

vec4 pow4(vec4 target, float value)
{
	return vec4(pow(target.x, value), pow(target.y, value), pow(target.z, value), pow(target.w, value));
}

vec3 pow3(vec3 target, float value)
{
	return vec3(pow(target.x, value), pow(target.y, value), pow(target.z, value));
}

vec3 abs3(vec3 target)
{
	return vec3(abs(target.x), abs(target.y), abs(target.z));
}

float saturate(float value)
{
	return clamp(value, 0.0f, 1.0f);
}

float3 saturate3(float3 value)
{
	return float3(saturate(value.x), saturate(value.y), saturate(value.z));
}

vec3 convertFloatVec3(float value)
{
	return vec3(value, value, value);
}

