#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "../inc/descriptors.inc"

#using in 		: VSOut_Color 		in
#using out 		: FSOut_Post	out

void main() 
{
	out.fragColor = in.color;
}