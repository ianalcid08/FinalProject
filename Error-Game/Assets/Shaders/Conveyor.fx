float4x4 gWorld : WORLD;
float4x4 gViewProjection : VIEWPROJECTION;
float gTime : TIME;
float3 gLightDir;

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

	output.lightDirection = gLightDir - worldPos.xyz;
	
	float2 newUV = input.uv;
	newUV[1] -= gTime;
	output.uv = newUV;

	return output; 
}

float4 myPixelShader(VS_OUTPUT input) : COLOR
{
	float4 LightColor = {1.0f, 1.0f, 1.0f, 1.0f};
	float4 ambientColor = {0.1f, 0.1f, 0.1f, 1.0f};
	float4 color = tex2D(gDiffuseSampler, input.uv);
	
	// Light calcs
	float3 ldir = normalize(input.lightDirection);
	float3 wnrm = {0,1.0f, 0};
	float4 tempNrm = tex2D(gNormalSampler, input.uv);
	wnrm.r = (tempNrm.r * 2.0f) - 1.0f;
	wnrm.g = (tempNrm.b * 2.0f) - 1.0f;
	wnrm.b = (tempNrm.g * 2.0f) - 1.0f;
	
	float darken = saturate(dot(wnrm,ldir));
	
	float4 colorMod;
	
	if(darken > 0)
	{
		colorMod = ambientColor + (LightColor*darken*darken*darken);
	}
	else
		colorMod = ambientColor;
	
	colorMod = saturate(colorMod);
		
	color = color * colorMod;
	return color;
}

VS_OUTPUT PoolVertShader(VS_INPUT input)
{
	VS_OUTPUT output;
	
	float4 worldPos = {input.untransformed_pos, 1.0f};
	worldPos = mul(worldPos, gWorld); 
	output.transformed_pos = mul(worldPos, gViewProjection);
	
	output.lightDirection = gLightDir;
	
	float2 newUV = input.uv;
	newUV[0] -= gTime;
	newUV[1] -= gTime * .5f;
	output.uv = newUV;

	return output; 
}

float4 LakeOFire(VS_OUTPUT input) : COLOR
{
	float4 color = tex2D(gDiffuseSampler, input.uv);
	
	float intensityMod = max(.8f, sin(gTime*0.5f));
	color.rgb*= intensityMod + .25f;
	color.gb *= intensityMod;
	
	return color;
}

technique Conveyor
{
    pass FirstPass
    {
        vertexShader = compile vs_3_0 myVertexShader();
        pixelShader  = compile ps_3_0 myPixelShader();
		cullmode = CCW;
    }
}

technique LakeofFire
{
    pass FirstPass
    {
        vertexShader = compile vs_3_0 PoolVertShader();
        pixelShader  = compile ps_3_0 LakeOFire();
		cullmode = CCW;
    }
}