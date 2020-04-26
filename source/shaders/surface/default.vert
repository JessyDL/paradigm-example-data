#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "../inc/math.inc"
#include "../inc/descriptors.inc"

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iCol;
layout(location = 2) in vec2 iTex;
layout(location = 3) in vec3 iNorm;
layout(location = 4) in mat4 INSTANCE_TRANSFORM;

layout(location = 0) out vec4 vsCol;
layout(location = 1) out vec2 vsTex;
layout(location = 2) out vec4 vsWPOS;
layout(location = 3) out vec4 vsViewVec;
layout(location = 4) out vec3 vsNormal;
layout(location = 5) out flat int vsInstanceIndex;

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
	vsCol = vec4(0);
	vsTex = iTex;
	vsInstanceIndex = gl_InstanceIndex;
	mat4 mNormal = ubo.data.modelMatrix * INSTANCE_TRANSFORM;
	vsNormal = normalize((mNormal * vec4(normalize(iNorm), 0)).xyz);
		
	vsWPOS.xyz = (ubo.data.modelMatrix * INSTANCE_TRANSFORM * vec4(iPos, 1.0)).xyz;
	
	gl_Position = ubo.data.WVP * vec4(vsWPOS.xyz, 1.0);
	
	vsViewVec.xyz = (ubo.data.viewPos.xyz - vsWPOS.xyz);
	vsViewVec.w = length(vsViewVec.xyz);
	vsViewVec.xyz = normalize(vsViewVec.xyz);
	vsWPOS.w = distance(vsWPOS.xyz, ubo.data.viewPos.xyz) /ubo.data.ScreenParams.w;	
}