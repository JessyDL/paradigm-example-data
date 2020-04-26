#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "../inc/math.inc"
#include "../inc/descriptors.inc"

layout(location = 0) in vec3 iPos;
layout(location = 1) in mat4 iModelMat;

layout(binding = 0, std140) uniform GLOBAL_DYNAMIC_WORLD_VIEW_PROJECTION_MATRIX
{
		FrameData data;
} ubo;

out gl_PerVertex 
{
    vec4 gl_Position;
};


void main() 
{
	float3 pos = (ubo.data.modelMatrix * iModelMat * vec4(iPos, 1.0)).xyz;
	
	gl_Position = ubo.data.WVP * vec4(pos, 1.0);
}