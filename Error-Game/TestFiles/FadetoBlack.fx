float FadeAmount;

struct VS_INPUT
{
    float3 untransformed_pos	: POSITION0;
};

struct VS_OUTPUT
{
    float4 transformed_pos	: POSITION0;
};

VS_OUTPUT myVertexShader(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 worldPos = {input.untransformed_pos, 1.0f};
	output.transformed_pos = worldPos;

	return output; 
}

float4 myPixelShader(VS_OUTPUT input) : COLOR
{
	float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	color.a *= FadeAmount;

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
