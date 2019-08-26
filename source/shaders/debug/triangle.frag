#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "../inc/descriptors.inc"

#using in 	: VSOut_ColorTex vs
#using descriptors 	: FSBindings_Simple

layout(location = 0) out vec4 outColor;

void main() 
{
	outColor = vec4(texture(GSampler, vs.tex).xyz,1.0);	
}