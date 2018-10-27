#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "../inc/math.inc"
#include "../inc/descriptors.inc"

#using in 			: VSIn 		in
#using out 			: VSOut 	out
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
	out.Color = vec4(0);
	out.Tex = in.Tex;
	out.InstanceIndex = gl_InstanceIndex;
	mat4 mNormal = ubo.data[pConstant.UBOIndex].modelMatrix * in.modelMat;
	out.Normal = normalize((mNormal * vec4(normalize(in.Normal), 0)).xyz);
		
	out.WPos.xyz = (ubo.data[pConstant.UBOIndex].modelMatrix * in.modelMat * vec4(in.Pos, 1.0)).xyz;
	
	gl_Position = ubo.data[pConstant.UBOIndex].WVP * vec4(out.WPos.xyz, 1.0);
	
	out.ViewVec.xyz = (ubo.data[pConstant.UBOIndex].viewPos.xyz - out.WPos.xyz);
	out.ViewVec.w = length(out.ViewVec.xyz);
	out.ViewVec.xyz = normalize(out.ViewVec.xyz);
	out.WPos.w = distance(out.WPos.xyz, ubo.data[pConstant.UBOIndex].viewPos.xyz) /ubo.data[pConstant.UBOIndex].ScreenParams.w;	
}