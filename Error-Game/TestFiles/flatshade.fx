// application exposed variables
float4x4 gWorldViewProjection : WORLDVIEWPROJECTION;
float4 gColor;

// vertex shader inputs (must match the vertex declaration in C++)
struct VS_INPUT
{
    float3 untransformed_pos	: POSITION0;
	float3 untransformed_norm	: NORMAL0;
};
// vertex shader outputs
struct VS_OUTPUT
{
    float4 transformed_pos	: POSITION0;
    float4 color_rgba 		: COLOR0;
};
// the vertex shader, run once for each vertex
VS_OUTPUT myVertexShader(VS_INPUT input)
{
	VS_OUTPUT output; // what we will output
	// transforming the incoming vertex into world, then camera, then clip space.
	output.transformed_pos = mul(float4(input.untransformed_pos, 1.0f), gWorldViewProjection);
	// per vertex shading, convert surface normal to color
	output.color_rgba = float4(input.untransformed_norm, 1.0f);
	// send data along to the pixel shader (will be interpolated)
	return output; 
}
// the pixel shader, each rasterized triangle's pixels will run through this 
float4 myPixelShader(float4 color : COLOR0) : COLOR
{
	// return our interpolated vertex color
	return gColor; // send pixel to screen/output buffer
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
		fillmode = wireframe;
    }
}