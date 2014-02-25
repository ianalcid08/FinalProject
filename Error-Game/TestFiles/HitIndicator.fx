float4x4 gWorldViewProj : WORLDVIEWPROJECTION;


struct VS_INPUT
{
	float3 Pos		:POSITION0;
	float3 untransformedNormal : NORMAL0;		
};
struct VS_OUTPUT
{
	float4 color_rgba		: COLOR0;
};
// vertex shader
VS_OUTPUT hitIndicatorVS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.color_rgba = float4(input.untransformed_norm, 1.0f);
	return output;
}
// pixel shader
float4 hitIndicatorPS(float4 color : COLOR0) : COLOR
{
	return color * 1.5f;
}
// technique
technique myTechnique
{
	pass FirstPass
    {
		// request what vertex and pixel shader are to be used, this can change for each pass.
        vertexShader = compile vs_2_0 myVertexShader();
        pixelShader  = compile ps_2_0 myPixelShader();
		// Setting a few of the many D3D renderstates via the effect framework
		ShadeMode = FLAT; // flat color interpolation across triangles
        FillMode = SOLID; // no wireframes, no point drawing.
        CullMode = CCW; // cull any counter-clockwise polygons.
    }
}