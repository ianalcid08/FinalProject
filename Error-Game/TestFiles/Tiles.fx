// application exposed variables
float4x4 gViewProjection : VIEWPROJECTION;

float3 gLightDir;
bool gNormalMapped;

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

texture2D gNormalTexture : DIFFUSE;
sampler gNormalSampler = 
sampler_state 
{
    Texture = <gNormalTexture>;
    FILTER = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

// vertex shader inputs (must match the vertex declaration in C++)
struct VS_INPUT
{
    float3 untransformed_pos	: POSITION0;
	float2 uv					: TEXCOORD0;
};

// vertex shader outputs
struct VS_OUTPUT
{
    float4 transformed_pos	: POSITION0;
	float2 uv				: TEXCOORD0;
	float3 lightDirection : TEXCOORD1;
};
// the vertex shader, run once for each vertex
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
// the pixel shader, each rasterized triangle's pixels will run through this 
float4 myPixelShader(VS_OUTPUT input) : COLOR
{
	float4 LightColor = {1.0f, 1.0f, 1.0f, 1.0f};
	float4 ambientColor = {0.1f, 0.1f, 0.1f, 1.0f};
	float4 color = tex2D(gDiffuseSampler, input.uv);
	
	// Light calcs
	float3 ldir = normalize(input.lightDirection);
	float4 tempNrm = {0,0,0,0};
	float3 wnrm;
	if(!gNormalMapped)
		wnrm = float3(0.0f, 1.0f, 0.0f);
	else
	{
		float4 tempNrm = tex2D(gNormalSampler, input.uv);
		wnrm.r = (tempNrm.r * 2.0f) - 1.0f;
		wnrm.g = (tempNrm.b * 2.0f) - 1.0f;
		wnrm.b = (tempNrm.g * 2.0f) - 1.0f;
	}
	
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
// Techniques are read in by the effect framework.
// They allow one to add variation to how a particular shader might be executed. 
technique myTechnique
{
    pass FirstPass
    {
		// request what vertex and pixel shader are to be used, this can change for each pass.
        vertexShader = compile vs_3_0 myVertexShader();
        pixelShader  = compile ps_3_0 myPixelShader();
		cullmode = CCW;
    }
}