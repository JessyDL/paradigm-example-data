#version 300 es

#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_shader_io_blocks : enable

#define float3 vec3
#define float4 vec4
#define float2 vec2
#define lerp mix
#define PI 3.1415926536f

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

// Compute barycentric coordinates (u, v, w) for
// point p with respect to triangle (a, b, c)
vec3 barycenter(vec3 p)
{
	vec3 a = vec3(1,0,0);
	vec3 b = vec3(0,1,0);
	vec3 c = vec3(0,0,1);
	
    vec3 v0 = b - a;
	vec3 v1 = c - a;
	vec3 v2 = p - a;
	
    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);
	
    float denom = d00 * d11 - d01 * d01;
	
	vec3 result = vec3(0);
    result.y = (d11 * d20 - d01 * d21) / denom;
    result.z = (d00 * d21 - d01 * d20) / denom;
    result.x = 1.0f - result.y - result.z;
	
	return result;
}

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTex;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outTex;

out gl_PerVertex 
{
    vec4 gl_Position;
};


void main() 
{
	outColor = vec4(inColor.xyz, 1.0f);		
	outTex = inTex;
	//vec3 position = (ubo.data[0].modelMatrix * vec4(inPosition.xyz, 1.0)).xyz;	
	//gl_Position = ubo.data[0].WVP * vec4(position, 1.0);
	
	gl_Position = vec4(inPosition, 1.0);
}