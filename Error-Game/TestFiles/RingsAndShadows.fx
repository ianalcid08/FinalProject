// application exposed variables
float4x4 gViewProjection : VIEWPROJECTION;
float4x4 gWorld;
float4 gColor;

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

// vertex shader inputs (must match the vertex declaration in C++)
struct VS_INPUT
{
	float3 untransformed_pos: POSITION0;
	float2 uv		: TEXCOORD0;
};

// vertex shader outputs
struct VS_OUTPUT
{
	float4 transformed_pos	: POSITION0;
	float2 uv		: TEXCOORD0;
};
// the vertex shader, run once for each vertex
VS_OUTPUT myVertexShader(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 worldPos = {input.untransformed_pos, 1.0f};
	worldPos = mul(worldPos,gWorld);
	output.transformed_pos = mul(worldPos, gViewProjection);

	output.uv = input.uv;

	return output; 
}
// the pixel shader, each rasterized triangle's pixels will run through this 
float4 myPixelShader(VS_OUTPUT input) : COLOR
{
	float4 color = tex2D(gDiffuseSampler, input.uv) * gColor;
	
	return color;
}
// Techniques are read in by the effect framework.
// They allow one to add variation to how a particular shader might be executed. 
technique myTechnique
{
    pass FirstPass
    {
        vertexShader = compile vs_3_0 myVertexShader();
        pixelShader  = compile ps_3_0 myPixelShader();
		cullmode = CCW;
    }
}