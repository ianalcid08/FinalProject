// application exposed variables
float4x4 gOffset;
float gRatio;
float gFade;
int gHit;
float4 gColor;

texture2D gDiffuseTexture : DIFFUSE;
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

struct VS_OUTPUT
{
	float4 transformed_pos	: POSITION0;
	float2 uv		: TEXCOORD0;
};

VS_OUTPUT myVertexShader(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 worldPos = {input.untransformed_pos, 1.0f};
	output.transformed_pos = mul(worldPos, gOffset);

	output.uv = input.uv;

	return output; 
}

float4 myPixelShader(VS_OUTPUT input) : COLOR
{
	float4 color;
	if(input.uv.x > gRatio)
	{
		color = tex2D(gBackSampler, input.uv);
		if(color.w > 0.75f)
		{
			color.w = gFade;
		}
		else
			discard;
	}
	else
	{
		color = tex2D(gDiffuseSampler, input.uv);
		if(color.w > 0.75f)
		{
			color.w = gFade;
		}
		else
			discard;
	}	

	if(gHit == 1)
	{
		color = 1 - color;
		color.a = color.a * 10.0f;
	}
	else if(gHit == 2)
	{
		float grey = (color.r + color.g + color.b) / 3;
		color.rgb = grey;
	}
		
	return color * gColor;
}

technique myTechnique
{
    pass FirstPass
    {
        vertexShader = compile vs_3_0 myVertexShader();
        pixelShader  = compile ps_3_0 myPixelShader();
    }
}