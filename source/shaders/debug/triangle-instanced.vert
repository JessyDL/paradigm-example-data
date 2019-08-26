#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "../inc/descriptors.inc"
#include "../inc/math.inc"

#using in 					: VSIn				in
#using out 					: VSOut_ColorTex 	vs
#using descriptors 	: VSBinding

out gl_PerVertex 
{
    vec4 gl_Position;
};


void main() 
{
	vs.color = vec4(in.color.xyz, 1.0f);		
	vs.tex = in.tex;
	vec3 position = (ubo.data[0].modelMatrix * in.modelMat * vec4(in.position.xyz, 1.0)).xyz;	
	gl_Position = ubo.data[0].WVP * vec4(position, 1.0);
}