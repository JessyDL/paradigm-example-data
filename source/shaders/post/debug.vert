#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "../inc/descriptors.inc"


layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iTex;

layout(location = 0) out vec2 vsTex;

out gl_PerVertex 
{
    vec4 gl_Position;
};

void main() 
{
	vsTex = iTex;
	vsTex.y = 1-vsTex.y;
	
	vec3 pos = iPos;
	pos.y = -pos.y;
	pos.x = pos.x * 2 - 1;
	pos.y = pos.y * 2 + 1;
	gl_Position =  vec4(pos, 1.0);
}