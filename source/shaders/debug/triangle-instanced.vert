#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "../inc/descriptors.inc"
#include "../inc/math.inc"

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iCol;
layout(location = 2) in vec2 iTex;
layout(location = 3) in mat4 INSTANCE_TRANSFORM;

layout(location = 0) out vec4 vsCol;
layout(location = 1) out vec2 vsTex;

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
	vsCol = vec4(iCol.xyz, 1.0f);		
	vsTex = iTex;
	vec3 position = (ubo.data.modelMatrix * INSTANCE_TRANSFORM * vec4(iPos.xyz, 1.0)).xyz;	
	gl_Position = ubo.data.WVP * vec4(position, 1.0);
}