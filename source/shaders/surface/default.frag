#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "../inc/descriptors.inc"
#include "../inc/math.inc"
#include "../inc/helpers.inc"

layout(location = 0) in vec4 vsCol;
layout(location = 1) in vec2 vsTex;
layout(location = 2) in vec4 vsWPOS;
layout(location = 3) in vec4 vsViewVec;
layout(location = 4) in vec3 vsNormal;
layout(location = 5) in flat int vsInstanceIndex;

layout(location = 0) out vec4 fsPos;
layout(location = 1) out vec4 fsNormal;
layout(location = 2) out vec4 fsAlbedo;
layout(location = 3) out vec4 fsAccumulation;

layout(binding = 1) uniform sampler2D albedoMap;
layout(binding = 2) uniform sampler2D metalicityMap;
layout(binding = 3) uniform sampler2D normalMap;
layout(binding = 4) uniform sampler2D roughnessMap;
layout(binding = 5) uniform samplerCube reflectMap;

layout(std430, binding = 6) buffer MaterialData
{
	material data[];
} mData;

void main() 
{
	fsPos = vsWPOS;
	vec2 tex = vsTex;
	//tex.y = 1-tex.y;
	vec3 normal = DecodeNormalMap(normalMap, tex, vsNormal, vsViewVec.xyz * vsViewVec.w);
	
	fsNormal = vec4(encodeWSNormals(normal), texture(metalicityMap, tex).x , texture(roughnessMap, tex).x);
	
	fsAlbedo = texture(albedoMap, tex) * mData.data[vsInstanceIndex].color;
	fsAccumulation = vec4(0);
}