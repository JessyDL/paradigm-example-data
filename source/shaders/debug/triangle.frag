#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in vec4 vsCol;
layout(location = 1) in vec2 vsTex;
layout(location = 0) out vec4 fsCol;

layout(binding = 1) uniform sampler2D GSampler;
void main() 
{
	fsCol = vec4(texture(GSampler, vsTex).xyz,1.0) * vsCol;	
}