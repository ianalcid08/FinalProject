// application exposed variables
float4x4 gWorld : WORLD;
float4x4 gViewProjection : VIEWPROJECTION;
float4x4 gBones[15];
int AmIHit;
int InvertedHit;
float randomNum;
float3 gLightDir;

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
    float3 untransformed_pos	: POSITION0;
	float3 untransformed_norm	: NORMAL0;
	float2 uv					: TEXCOORD0;
	uint bone					: BLENDINDICES0;
};

// vertex shader outputs
struct VS_OUTPUT
{
    float4 transformed_pos	: POSITION0;
	float3 transformed_norm : TEXCOORD0;
	float2 uv				: TEXCOORD1;
	float3 lightDirection : TEXCOORD2;
};
// the vertex shader, run once for each vertex
VS_OUTPUT myVertexShader(VS_INPUT input)
{
	VS_OUTPUT output; // what we will output
   	// TODO: compute world vertex position

    // TODO: transform the incoming world vertex into camera, then clip space.
	float4 localPos = {input.untransformed_pos, 1.0f};
	localPos = mul(localPos, gBones[input.bone]);
	float4 worldPos = mul(localPos, gWorld);
	output.transformed_pos = mul(worldPos, gViewProjection);
	
	// Transforming normals
	float4 worldnorm = float4(input.untransformed_norm,0);
	worldnorm = mul(worldnorm, gBones[input.bone]);
	worldnorm = mul(worldnorm, gWorld);
	output.transformed_norm = worldnorm.xyz;

	output.lightDirection = gLightDir - worldPos.xyz;
	
	// TODO: transfer UVs
	output.uv = input.uv;

	// send data along to the pixel shader (will be interpolated)
	return output; 
}
// the pixel shader, each rasterized triangle's pixels will run through this 
float4 myPixelShader(VS_OUTPUT input) : COLOR
{
	float4 LightColor = {1.0f, 1.0f, 1.0f, 1.0f};
	float4 ambientColor = {0.1f, 0.1f, 0.1f, 1.0f};
	// TODO: return texture color
	float4 color = tex2D(gDiffuseSampler, input.uv);
	
	// Light Dir and normals
	float3 ldir = normalize(input.lightDirection);
	float3 wnrm = normalize(input.transformed_norm);	
	float darken = saturate(dot(wnrm,ldir));
	
	float4 colorMod;
	
	if(darken > 0)
	{
		colorMod = ambientColor + (LightColor*darken);
	}
	else
		colorMod = ambientColor;
	
	colorMod = saturate(colorMod);
		
	color = color * colorMod;
	
	
	if(AmIHit == 0)
		return color;
	else if(AmIHit == 2)
	{
		color.a -= randomNum;
		return color;
	}
	else if(AmIHit == 3)
	{
		return color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else if(AmIHit == 5)
	{
		 //Gatling - orange
		 return color * float4(2.0f, 1.0f, 0.5f, 1.0f);
	}
	else if(AmIHit == 6)
	{
		//Pulse - purple
		return color * float4(2.0f, 1.0f, 2.0f, 1.0f);
	}
	else if(AmIHit == 7)
	{
		//Tesla - Teal
		return color * float4(1.0f, 2.0f, 2.0f, 1.0f);
	}
	else if(AmIHit == 8)
	{
		color.r = 1.0f - color.r;
		color.g = 1.0f - color.g;
		color.b = 1.0f - color.b;
		return color;
	}
	else if(AmIHit == 9)
	{
		color.a -= randomNum;
		color.r = 1.0f - color.r;
		color.g = 1.0f - color.g;
		color.b = 1.0f - color.b;
		return color;
	}

	else if(AmIHit == 10)
	{
		float4 temp = tex2D(gDiffuseSampler, input.uv);
		if(temp.r >= 0.75f && temp.g <= 0.25 && temp.b >= 0.75f)
			color = float4(1.0f, 0.0f, 0.0f, 1.0f);
		return color;
	}
	
	else if(AmIHit == 11)
	{
		float4 temp = tex2D(gDiffuseSampler, input.uv);
		if(temp.r >= 0.75f && temp.g <= 0.25 && temp.b >= 0.75f)
			color = float4(1.0f, 1.0f, 1.0f, 1.0f) * colorMod;
		return color;
	}
	else
		return color * 2.0f;		
}
// Techniques are read in by the effect framework.
// They allow one to add variation to how a particular shader might be executed. 
technique myTechnique
{
	// Each pass in the technique corresponds to a single iteration over all 
	// verticies sent during a draw call and the subsequently created pixels. 
	// If we had a special effect that required us to draw our geometry and pixels more than once,
	// we could acheive this by adding more passes across our vertex and pixel data.
    pass FirstPass
    {
		// request what vertex and pixel shader are to be used, this can change for each pass.
        vertexShader = compile vs_3_0 myVertexShader();
        pixelShader  = compile ps_3_0 myPixelShader();
		cullmode = CCW;
    }
}