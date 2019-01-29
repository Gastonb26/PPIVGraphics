//rule of three:
//three things must match
// 1. C++ Vertex Struct
// 2. Input Layout
// 3. HLSL Vertex Struct
//static const GrayScaleIntensity = { 0.299f, 0.587f, 0.114f };

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
	float3 lightPos; 
	float Intensity; 
	float3 spotPos; 
}

struct VS_INPUT
{
	float4 Pos : POSITION; 
	float3 Norm : NORMAL;
	float3 Tex : TEXCOORD0; 
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION; // system value
	float3 Norm : NORMAL;
	float3 Tex : TEXCOORD0; 
	float4 WorldPosition : POSITION;
};


float4 PS(PS_INPUT input) : SV_Target
{
	return vOutputCol;
}

