#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "../inc/descriptors.inc"

layout(location = 0) in vec2 vsTex;
layout(location = 0) out vec4 frag;

layout(binding = 1) uniform sampler2D GSampler;

void main() 
{
	frag = texture(GSampler, vsTex);
}	