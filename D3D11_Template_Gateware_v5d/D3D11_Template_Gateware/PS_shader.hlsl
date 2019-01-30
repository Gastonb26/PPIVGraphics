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
	float4 WorldPosition : POSITION;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION; // system value
	float3 Norm : NORMAL;
	float3 Tex : TEXCOORD0; 
};

//}

float4 PS(PS_INPUT input) : SV_Target
{
	float4 finalColor = 0;

	//do NdotL lighting for 2 lights
	for (int i = 0; i<2; i++)
	{
		finalColor += 5 * (saturate(dot((float3)vLightDir[i],input.Norm) * vLightColor[i]));
	}
	finalColor.a = 1;
	return finalColor;
}
//
//float4 PS(VS_INPUT input) : SV_Target
//{
//	
//	return vOutputCol;
//}
