float4x4 gViewProjection : VIEWPROJECTION;

float3 gLightDir;
float4 gAmbient;

texture2D gDiffuseTexture : DIFFUSE;
sampler gDiffuseSampler = 
sampler_state 
{
    Texture = <gDiffuseTexture>;
    FILTER = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

texture2D gNormalTexture : DIFFUSE;
sampler gNormalSampler = 
sampler_state 
{
    Texture = <gNormalTexture>;
    FILTER = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VS_INPUT
{
    float3 untransformed_pos	: POSITION0;
	float2 uv					: TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 transformed_pos	: POSITION0;
	float2 uv				: TEXCOORD0;
	float3 lightDirection : TEXCOORD1;
};

VS_OUTPUT myVertexShader(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 worldPos = {input.untransformed_pos, 1.0f};
	output.transformed_pos = mul(worldPos, gViewProjection);
	
	// light direction
	output.lightDirection = gLightDir - worldPos.xyz;

	output.uv = input.uv;

	return output; 
}

float4 myPixelShader(VS_OUTPUT input) : COLOR
{
	float4 color = tex2D(gDiffuseSampler, input.uv);
	
	// Light calcs
	float3 ldir = normalize(input.lightDirection);
	float3 wnrm;
	
	float4 tempNrm = tex2D(gNormalSampler, input.uv);
	wnrm.r = (tempNrm.r * 2.0f) - 1.0f;
	wnrm.g = (tempNrm.b * 2.0f) - 1.0f;
	wnrm.b = (tempNrm.g * 2.0f) - 1.0f;
	
	float darken = saturate(dot(wnrm,ldir));
	
	float4 colorMod;
	
	if(darken > 0)
	{
		colorMod = gAmbient + (darken*darken*darken);
	}
	else
		colorMod = gAmbient;
	
	colorMod = saturate(colorMod);
		
	color = color * colorMod;
	return color;
}

VS_OUTPUT StaticVertexShader(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 worldPos = {input.untransformed_pos, 1.0f};
	output.transformed_pos = mul(worldPos, gViewProjection);
	
	// light direction
	output.lightDirection = gLightDir;

	output.uv = input.uv;

	return output; 
}

float4 StaticPixelShader(VS_OUTPUT input) : COLOR
{
	float4 color = tex2D(gDiffuseSampler, input.uv);
	return color;
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

technique StaticScreen
{
    pass FirstPass
    {
        vertexShader = compile vs_3_0 StaticVertexShader();
        pixelShader  = compile ps_3_0 StaticPixelShader();
		cullmode = CCW;
    }
}