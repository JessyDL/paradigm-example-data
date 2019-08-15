#version 300 es

#extension GL_ARB_separate_shader_objects : enable

precision highp float;
precision highp int;
precision lowp sampler2D;
precision lowp samplerCube;

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec2 inTex;

uniform sampler2D	GSampler;

//#using in 	: VSOut_Color 	in
//#using descriptors 	: FSBindings_Simple bind

layout(location = 0) out vec4 outColor;

void main() 
{
	outColor = vec4(texture(GSampler, inTex).xyz,1.0);
	//outColor = inColor;
}