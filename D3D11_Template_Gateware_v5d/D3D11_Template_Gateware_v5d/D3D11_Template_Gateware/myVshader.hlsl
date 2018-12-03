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
}

struct InputVertex
{
	float4 xyzw : POSITION; 
	float4 rgba : COLOR; 
};

struct OutputVertex
{
	float4 xyzw : SV_POSITION; // system value
	float4 rgba : COLOR; 
};

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;
	output.xyzw = input.xyzw; 

	output.xyzw = mul(input.xyzw, World);
	output.xyzw = mul(output.xyzw, View);
	output.xyzw = mul(output.xyzw, Projection);

	output.rgba = input.rgba;

	return output;
}

//OutputVertex main(InputVertex input)
//{
//	OutputVertex output = (OutputVertex)0;
//	//output.xyzw = input.xyzw; 
//	// DO math here
//	output.xyzw = mul(input.xyzw, mWorld);
//	output.xyzw = mul(output.xyzw, mView);
//	output.xyzw = mul(output.xyzw, mProjection);
//
//	return output; 
//}

//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}