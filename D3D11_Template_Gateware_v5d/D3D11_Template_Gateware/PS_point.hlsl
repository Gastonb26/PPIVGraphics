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
	float3 lightPos; 
	float pointIntensity; 
	float3 spotPos; 
	float spotIntensity;
	float4 spotCol; 
	float3 spotDirection; 
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
	//Both lights Use
	float3 lightDir = normalize(spotPos - input.WorldPosition);
	
	
	//SpotLight  //TODO change voutPutColor to point Color and add Spot Color to constant buffer? 
	float lightRat = saturate(dot(lightDir, input.Norm));
	float3 coneDir = normalize(spotDirection);

	input.Norm = normalize(input.Norm);
	float surfaceRat = saturate(dot(-lightDir, coneDir));
	float spotFac = (surfaceRat > .90f) ? 1 : 0;
	float spotAtt = 1.0f - saturate((0.96f - surfaceRat) / (0.96f - .90f));
	float4 spotFinal = spotIntensity*(spotFac * lightRat * spotCol * txDiffuse.Sample(samLinear, input.Tex) * (spotAtt*spotAtt));
	//float4 spotFinal = Intensity*(spotFac * lightRat * spotColor * txDiffuse.Sample(samLinear, input.Tex) * (spotAtt*spotAtt));
	
	//PointLight
	lightDir = normalize(lightPos - input.WorldPosition);
	float lightRatio = saturate(dot(lightDir, input.Norm));	
	float attenuation = 1.0f - saturate(length(lightPos - input.WorldPosition) / 9.99f);
	float4 pointFinal = pointIntensity *(saturate(lightRatio *  vOutputCol  * txDiffuse.Sample(samLinear, input.Tex) * (attenuation*attenuation)));


	return spotFinal + pointFinal; 

	//SLIDE NOTES: SpotLight 
	//SURFACERATIO = CLAMP(DOT(-LIGHTDIR, CONEDIR))
	//SPOTFACTOR = (SURFACERATIO > CONERATIO) ? 1 : 0
	//LIGHTRATIO = CLAMP(DOT(LIGHTDIR, SURFACENORMAL))
	//RESULT = SPOTFACTOR * LIGHTRATIO * LIGHTCOLOR * SURFACECOLOR
	//ATTENUATION = 1.0 – CLAMP((INNERCONERATIO - SURFACERATIO) / (INNERCONERATIO – OUTERCONERATIO))

}

