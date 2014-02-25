// application exposed variables
float4x4 gWorld : WORLD;
float4x4 gViewProjection : VIEWPROJECTION;
float4 gColor: COLOR;

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
	
	output.uv.x = (localPos.x * 0.5f + 0.5f);
	output.uv.y = (localPos.z * 0.5f + 0.5f);

	return output;
}

float4 myPixelShader(VS_OUTPUT input) : COLOR
{
	float4 color = tex2D(gDiffuseSampler, input.uv);
	if(color.r > 0.7f && color.g > 0.7f && color.b > 0.7f && color.a > 0.95f)
	{
		float4 nColor = float4(1.0f,1.0f,1.0f,1.0f) - gColor;
		nColor.a = gColor.a;
		//color = color * nColor;
		return color * 2.0f;
	}
	else
	{
		color = color * gColor;
		return color * 2.0f;
	}
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