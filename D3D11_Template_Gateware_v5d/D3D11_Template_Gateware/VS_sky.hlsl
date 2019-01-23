//rule of three:
//three things must match
// 1. C++ Vertex Struct
// 2. Input Layout
// 3. HLSL Vertex Struct


//Texture2D txDiffuse : register(t0);
//SamplerState samLinear : register(s0);

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
	float3 Tex : TEXCOORD0; 
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION; // system value
	float3 Norm : NORMAL;
	float4 WorldPosition : POSITION;
	float3 Tex : TEXCOORD0; 
};


PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Norm = mul(float4(input.Norm, 0), World).xyzw;

	output.Tex = input.Pos;

	return output;
}

