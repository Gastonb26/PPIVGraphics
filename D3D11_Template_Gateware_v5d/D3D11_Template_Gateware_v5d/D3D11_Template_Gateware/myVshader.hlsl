//rule of three:
//three things must match
// 1. C++ Vertex Struct
// 2. Input Layout
// 3. HLSL Vertex Struct

struct InputVertex
{
	float4 xyzw : POSITION; 
	float4 rgba : COLOR; 
};

struct OutputVertex
{
	float4 xyzw : SV_POSITION; // system value
	float4 rgba : outCOLOR; 
};

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;
	output.xyzw = input.xyzw; 
	// DO math here

	return output; 
}

//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}