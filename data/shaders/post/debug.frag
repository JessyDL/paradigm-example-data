#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "../inc/descriptors.inc"

#using in 		: VSOut_T 		in
#using out 		: FSOut_Post	out
#using descriptors 	: FSBindings_Simple

void main() 
{
	out.fragColor = texture(GSampler, in.tex);
}	