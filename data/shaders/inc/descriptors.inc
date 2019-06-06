
attribute VSIn
{
	location 0 vec3 position 	( buffer: GEOMETRY_VERTEX_POSITION 	rate: vertex   attribute: {106,0} );
	location 1 vec3 color 			( buffer: GEOMETRY_VERTEX_COLOR   	rate: vertex   attribute: {106,0} );
	location 2 vec2 tex 				( buffer: GEOMETRY_VERTEX_TEX      	rate: vertex   attribute: {103,0} );
	location 3 mat4x4 modelMat	( buffer: INSTANCE_TRANSFORM       	rate: instance attribute: {109,0}, {109,16}, {109,32}, {109,48} );
};

attribute VSIn_full
{
	location 0 vec3 position 	( buffer: GEOMETRY_VERTEX_POSITION 	rate: vertex   attribute: {106,0} );
	location 1 vec3 color 			( buffer: GEOMETRY_VERTEX_COLOR   	rate: vertex   attribute: {106,0} );
	location 2 vec2 tex 				( buffer: GEOMETRY_VERTEX_TEX      	rate: vertex   attribute: {103,0} );
	location 3 vec3 normal 			( buffer: GEOMETRY_VERTEX_NORMAL   	rate: vertex   attribute: {106,0} );
	location 4 mat4x4 modelMat	( buffer: INSTANCE_TRANSFORM       	rate: instance attribute: {109,0}, {109,16}, {109,32}, {109,48} );
};


attribute VSIn_PT
{
	location 0 vec3 pos 				( buffer: GEOMETRY_VERTEX_POSITION 	rate: vertex   attribute: {106,0} );
	location 1 vec2 tex 				( buffer: GEOMETRY_VERTEX_TEX 				rate: vertex   attribute: {103,0} );
};

attribute VSIn_P
{
	location 0 vec3 pos 				( buffer: GEOMETRY_VERTEX_POSITION 	rate: vertex   attribute: {106,0} );
	location 1 mat4x4 modelMat	( buffer: INSTANCE_TRANSFORM       	rate: instance attribute: {109,0}, {109,16}, {109,32}, {109,48} );
};


attribute VSIn_PCT
{
	location 0 vec3 position 	( buffer: GEOMETRY_VERTEX_POSITION 	rate: vertex   attribute: {106,0} );
	location 1 vec3 color 			( buffer: GEOMETRY_VERTEX_COLOR   	rate: vertex   attribute: {106,0} );
	location 2 vec2 tex 				( buffer: GEOMETRY_VERTEX_TEX      	rate: vertex   attribute: {103,0} );
};

attribute VSIn_PTM
{
	location 0 vec3 Pos 					( buffer: GEOMETRY_VERTEX_POSITION 	rate: vertex   	attribute: {106,0} );
	location 1 vec2 Tex 					( buffer: GEOMETRY_VERTEX_TEX 				rate: vertex   	attribute: {103,0} );
	location 2 mat4x4 modelMat 	( buffer: INSTANCE_POSITION 					rate: instance 	attribute: {109,0}, {109,16}, {109,32}, {109,48} );
};

attribute VSIn_PNM
{
	location 0 vec3 Pos 					( buffer: GEOMETRY_VERTEX_POSITION  rate: vertex   	attribute: {106,0} );
	location 1 vec3 Normal 				( buffer: GEOMETRY_VERTEX_NORMAL 		rate: vertex   	attribute: {106,0} );
	location 2 mat4x4 modelMat 	( buffer: INSTANCE_POSITION 					rate: instance 	attribute: {109,0}, {109,16}, {109,32}, {109,48} );
};

interface VSOut
{
	location 0 vec4 Color;
	location 1 vec2 Tex;
	location 2 vec4 WPos;
	location 3 vec4 ViewVec;
	location 4 vec3 Normal;
	location 5 int  InstanceIndex ( in: "flat" );
};

interface VSOut_T
{
	location 0 vec2 tex;
};

interface VSOut_Post
{
	location 0 vec2 Tex;
	location 1 vec3 ViewPos;
	location 2 vec4 ScreenParams;
	location 3 vec4 FogColor;
	location 4 vec3 ViewDir;
	location 5 vec3 CamViewDir;
};

interface VSOut_Debug
{
	location 0 vec4 Color;
	location 1 vec4 WPos;
	location 2 vec3 Normal;
	location 3 int  InstanceIndex  ( in: "flat" );
};

interface VSOut_ColorTex
{
	location 0 vec4 color;
	location 1 vec2 tex;
};


interface VSOut_Color
{
	location 0 vec4 color;
};

interface VSOut_Unlit
{
	location 0 vec2 Tex;
	location 1 int  InstanceIndex  ( in: "flat" );
};


struct FrameData
{
	mat4 clipMatrix;
	mat4 projectionMatrix;
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 WVP;
	mat4 VP;
	vec4 ScreenParams;
	vec4 GameTimer;
	vec4 Parameters;
	vec4 FogColor;
	vec4 viewPos;
	vec4 viewDir;
};


descriptor VSBinding
{
	binding 0 UBO ubo ( bind: "uniform" type: ubo buffer:GLOBAL_WORLD_VIEW_PROJECTION_MATRIX )
	{
		FrameData data[128];
	};
};

descriptor VSBinding_Post
{
	binding 0 UBO ubo ( bind: "uniform" type: ubo buffer:GLOBAL_WORLD_VIEW_PROJECTION_MATRIX)
	{
		FrameData data[128];
	};
	binding 9 MainCameraUBO camUBO ( bind: "uniform" type: ubo buffer:LIGHT_MATRIX)
	{
		mat4 clipMatrix;
		mat4 projectionMatrix;
		mat4 modelMatrix;
		mat4 viewMatrix;
		mat4 WVP;
		mat4 VP;
		vec4 ScreenParams;
		vec4 GameTimer;
		vec4 Parameters;
		vec4 FogColor;
		vec4 viewPos;
		vec4 viewDir;
	};
};


interface FSOut
{
	location 0 vec4 Position;
	location 1 vec4 Normal;
	location 2 vec4 Albedo;
	location 3 vec4 Accumulation;
};

interface FSOut_Post
{
	location 0 vec4 fragColor;
};

struct material
{
	vec4 color;
};

descriptor FSBindings
{
	binding 1 sampler2D 		albedoMap 		( bind: "uniform" type: combined_sampler default: {1,0,0.5,1} );
	binding 2 sampler2D 		metalicityMap	( bind: "uniform" type: combined_sampler default: {1,0,0,1} );
	binding 3 sampler2D 		normalMap		( bind: "uniform" type: combined_sampler default: {1,0,0,1} );
	binding 4 sampler2D 		roughnessMap	( bind: "uniform" type: combined_sampler default: {1,0,0,1} );
	binding 5 samplerCube 		reflectMap		( bind: "uniform" type: combined_sampler default: {1,0,0,1} );
	binding 6 MaterialData 		mData			( bind: "buffer"  type: ssbo buffer: MATERIAL_DATA default: {0,0,0,0} )
	{
		material data[];
	};
};


struct Light 
{
	vec4 color;
	vec4 component1;
	vec4 component2;
};

descriptor FSBindings_Post
{
	binding 1 sampler2D 	GPosition 		( bind: "uniform" type: combined_sampler );
	binding 2 sampler2D 	GNormal 		( bind: "uniform" type: combined_sampler );
	binding 3 sampler2D 	GAlbedo 		( bind: "uniform" type: combined_sampler );
	binding 4 sampler2D 	GAccumulation 	( bind: "uniform" type: combined_sampler );
	binding 5 UBO 			lightUBO		( bind: "uniform" type: ubo buffer: GLOBAL_LIGHT_BUFFER )
	{
		Light lights[64];
	};
	binding 6 samplerCube 	reflectMap 			( bind: "uniform" type: combined_sampler );
	binding 7 sampler2D 	samplerShadowMap 	( bind: "uniform" type: combined_sampler );
	binding 8 MVP 			mvp					( bind: "uniform" type: ubo buffer: GLOBAL_LIGHT_BUFFER )
	{
		mat4 clipMatrix;
		mat4 projectionMatrix;
		mat4 modelMatrix;
		mat4 viewMatrix;
		mat4 WVP;
		mat4 VP;
	};
};

descriptor FSBindings_Simple
{
	binding 1 sampler2D 	GSampler ( bind: "uniform" type: combined_sampler default: {1,0,0.5,1} );
};

descriptor FSBindings_TexMat
{
	binding 0 sampler2D 		GSampler 		( bind: "uniform" 	type: combined_sampler default: {1,0,0.5,1});
	binding 1 MaterialData 	mData				( bind: "buffer" 	type: ssbo 		buffer: MATERIAL_DATA default: {0,0,0,0})
	{
		material data[];
	};
};