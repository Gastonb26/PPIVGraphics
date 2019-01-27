//rule of three:
//three things must match
// 1. C++ Vertex Struct
// 2. Input Layout
// 3. HLSL Vertex Struct


Texture2D txDiffuse : register(t0);
Texture2D txEmissive : register(t1); 
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


float4 main(PS_INPUT input) : SV_Target
{
	float4 outColor = 0; 
	float4 emissiveTexture = txEmissive.Sample(samLinear, input.Tex); 


	input.Norm = normalize(input.Norm);
	float3 lightDir = normalize(lightPos - input.WorldPosition);

	float lightRatio = saturate(dot(lightDir, input.Norm));

	float attenuation = 1.0f - saturate(length(lightPos - input.WorldPosition) / 9.99f);

	return saturate(lightRatio *  vOutputCol  * (attenuation*attenuation));
}

//return saturate(lightRatio *  vOutputCol * txDiffuse.Sample(samLinear, input.Tex)  * (attenuation*attenuation));
