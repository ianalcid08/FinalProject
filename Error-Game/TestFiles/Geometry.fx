//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
float4 g_Color;			    // Color to apply to texture
texture g_MeshTexture;              // Color texture for mesh
float4x4 g_mViewProjection : VIEWPROJECTION;	    // ViewProjection matrix
float4x4 g_Bone;
float4x4 g_Translate;
float4x4 g_Rotate;
float4x4 g_Scale;



//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler MeshTextureSampler = 
sampler_state
{
	Texture = <g_MeshTexture>;
	FILTER = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position
    float2 TextureUV  : TEXCOORD0;  // vertex texture coords 
};


//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( float3 vPos : POSITION,
                         float2 vNormal : NORMAL0,
			 float2 vUV : TEXCOORD0,
			 float3 vBone : BLENDINDICES0 )
{
    VS_OUTPUT Output;
    
    // Transform the position from object space to homogeneous projection space
	float4 worldPos = {vPos,1.0f};
	worldPos = mul(worldPos,g_Bone);
	worldPos = mul(worldPos,g_Scale);
	worldPos = mul(worldPos,g_Rotate);
	worldPos = mul(worldPos,g_Translate);
	worldPos = mul(worldPos, g_mViewProjection);
    Output.Position = worldPos;

    // Just copy the texture coordinate through
    Output.TextureUV = vUV; 
    
    return Output;    
}


//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
// color with diffuse material color
//--------------------------------------------------------------------------------------
float4 RenderScenePS( VS_OUTPUT In ) : COLOR
{ 
    // Lookup mesh texture and modulate it with diffuse
    float4 color = tex2D(MeshTextureSampler, In.TextureUV) * g_Color;

    return color;
}


//--------------------------------------------------------------------------------------
// Renders scene 
//--------------------------------------------------------------------------------------
technique RenderScene
{
    pass P0
    {          
        vertexshader = compile vs_3_0 RenderSceneVS();
        pixelshader  = compile ps_3_0 RenderScenePS(); 
		//cullmode = NONE;
    }
}
