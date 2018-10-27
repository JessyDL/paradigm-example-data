#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "../inc/descriptors.inc"
#include "../inc/math.inc"
#include "../inc/helpers.inc"

#using in 			: VSOut in
#using out 			: FSOut out
#using descriptors 	: FSBindings

void main() 
{
	out.Position = in.WPos;
	vec2 tex = in.Tex;
	//tex.y = 1-tex.y;
	vec3 normal = DecodeNormalMap(normalMap, tex, in.Normal, in.ViewVec.xyz * in.ViewVec.w);
	
	out.Normal = vec4(encodeWSNormals(normal), texture(metalicityMap, tex).x , texture(roughnessMap, tex).x);
	
	out.Albedo = texture(albedoMap, tex) * mData.data[in.InstanceIndex].color;
	out.Accumulation = vec4(0);
}