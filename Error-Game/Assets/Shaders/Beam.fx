float4x4 gWorld : WORLD;
float4x4 gViewProjection : VIEWPROJECTION;
float4 gColor: COLOR;

texture2D gDiffuseTexture : DIFFUSE;
sampler gDiffuseSampler = 
sampler_state 
{
    Texture = <gDiffuseTexture>;
    FILTER = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VS_INPUT
{
    float3 untransformed_pos	: POSITION0;
	float3 untransformed_norm	: NORMAL0;
	float2 uv					: TEXCOORD0;
	uint bone					: BLENDINDICES0;
};

struct VS_OUTPUT
{
    float4 transformed_pos	: POSITION0;
	float2 uv				: TEXCOORD1;
};

VS_OUTPUT myVertexShader(VS_INPUT input)
{
	VS_OUTPUT output;
	float4 localPos = {input.untransformed_pos, 1.0f};
	float4 worldPos = mul(localPos, gWorld);
	output.transformed_pos = mul(worldPos, gViewProjection);
	
	output.uv.x = input.untransformed_pos.x;
	output.uv.y = input.untransformed_pos.z;

	return output; 
}

float4 myPixelShader(VS_OUTPUT input) : COLOR
{
	float2 modColor = input.uv;
	modColor.x = modColor.x * 0.5f + 0.5f;
	float4 color = tex2D(gDiffuseSampler, modColor);
	color.a *= 0.5f - abs(input.uv.x);
	if(color.r > 0.65f && color.g > 0.65f && color.b > 0.65f)
	{
		float4 nColor = float4(1.0f,1.0f,1.0f,1.0f) - gColor;
		nColor.a = color.a;
		return color * nColor;
	}
	else
		return color * gColor;
}
 
technique myTechnique
{
    pass FirstPass
    {
        vertexShader = compile vs_3_0 myVertexShader();
        pixelShader  = compile ps_3_0 myPixelShader();
		cullmode = CCW;
    }
}