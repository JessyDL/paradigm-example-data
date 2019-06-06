#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "../inc/math.inc"
#include "../inc/descriptors.inc"

#using in 			: VSIn_P 		in
#using descriptors 	: VSBinding

layout (push_constant) uniform pushConstants
{
	uint UBOIndex;
} pConstant;

out gl_PerVertex 
{
    vec4 gl_Position;
};


void main() 
{
	float3 pos = (ubo.data[pConstant.UBOIndex].modelMatrix * in.modelMat * vec4(in.pos, 1.0)).xyz;
	
	gl_Position = ubo.data[pConstant.UBOIndex].WVP * vec4(pos, 1.0);
}