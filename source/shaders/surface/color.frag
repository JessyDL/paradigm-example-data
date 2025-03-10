#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "../inc/descriptors.inc"

layout(location = 0) in vec4 vsCol;
layout(location = 1) in vec2 vsTex;
layout(location = 0) out vec4 fsCol;

layout(binding = 2) uniform MaterialData
{
	vec4 color;
} mData;

void main() 
{
	fsCol = mData.color;
}