//rule of three:
//three things must match
// 1. C++ Vertex Struct
// 2. Input Layout
// 3. HLSL Vertex Struct

cbuffer ConstantBuffer : register (b0)
{
	matrix World; 
	matrix View; 
	matrix Projection;
	float4 vLightDir;
	float4 vLightColor;
	float4 vOutputCol;
}

struct InputVertex
{
	float4 xyzw : POSITION; 
	float3 Norm : NORMAL;
	float4 rgba : COLOR; 
};

struct OutputVertex
{
	float4 xyzw : SV_POSITION; // system value
	float4 Norm : NORMAL; 
	float4 rgba : COLOR; 
};

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;
	output.xyzw = mul(input.xyzw, World);
	output.xyzw = mul(output.xyzw, View);
	output.xyzw = mul(output.xyzw, Projection);

	output.Norm = mul( float4(input.Norm, 1), World).xyz;

	output.rgba = input.rgba;

	return output;
}

float4 PS(OutputVertex output) : SV_Target
turn finalColor;
}
