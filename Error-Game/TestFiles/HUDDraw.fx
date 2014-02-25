// application exposed variables
float4x4 gWorld : WORLD;
float4x4 gViewProjection : VIEWPROJECTION;
float gRatio;

// texture to be used
texture2D gDiffuseTexture : DIFFUSE;
// the matching sampler
sampler gDiffuseSampler = 
sampler_state 
{
	Texture = <gDiffuseTexture>;
	FILTER = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

texture2D gBackTexture;
sampler gBackSampler = 
sampler_state 
{
	Texture = <gBackTexture>;
	FILTER = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_INPUT
{
	float3 untransformed_pos	: POSITION0;
	float2 uv			: TEXCOORD0;
};

// vertex shader outputs
struct VS_OUTPUT
{
	float4 transformed_pos	: POSITION0;
	float2 uv		: TEXCOORD0;
};

VS_OUTPUT myVertexShader(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 localPos = {input.untransformed_pos, 1.0f};
	float4 worldPos = mul(localPos, gWorld);
	output.transformed_pos = mul(worldPos, gViewProjection);

	output.uv = input.uv;

	return output; 
}

float4 myPixelShader(VS_OUTPUT input) : COLOR
{
	float4 color;
	if(1 - input.uv.y > gRatio)
		color = tex2D(gBackSampler, input.uv);
	else
		color = tex2D(gDiffuseSampler, input.uv);
		
	return color;
}

technique myTechnique
{
    pass FirstPass
    {
		// request what vertex and pixel shader are to be used, this can change for each pass.
        vertexShader = compile vs_3_0 myVertexShader();
        pixelShader  = compile ps_3_0 myPixelShader();
    }
}