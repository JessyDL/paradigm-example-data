#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "../inc/descriptors.inc"

#using in 	: VSIn_PT 		in
#using out  : VSOut_T	 	out

out gl_PerVertex 
{
    vec4 gl_Position;
};

void main() 
{
	out.tex = in.tex;
	out.tex.y = 1-out.tex.y;
	
	vec3 pos = in.pos;
	pos.y = -pos.y;
	pos.x = pos.x * 2 - 1;
	pos.y = pos.y * 2 + 1;
	gl_Position =  vec4(pos, 1.0);
}