//rule of three:
//three things must match
// 1. C++ Vertex Struct
// 2. Input Layout
// 3. HLSL Vertex Struct


Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer ConstantBuffer : register (b0)
{
	matrix World; 
	matrix View; 
	matrix Projection;
	float4 vLightDir[2];
	float4 vLightColor[2];
	float4 vOutputCol;
}

struct VS_INPUT
{
	float4 Pos : POSITION; 
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD0; 
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION; // system value
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD0; 
};

//}

float4 PS(PS_INPUT input) : SV_Target
{
	return txDiffuse.Sample(samLinear ,input.Tex);
}