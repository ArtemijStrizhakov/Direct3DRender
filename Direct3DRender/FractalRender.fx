//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
}

struct VS_INPUT
{
	float4 Pos : SV_POSITION;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm : NORMAL;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = input.Pos;

	float scale = 0.00003;
	float centerx = -0.745428;
	float centery = 0.113009;

	float cx = centerx - (output.Pos.x)*scale;
	float cy = centery - (output.Pos.y)*scale;

	float zx = cx;
	float zy = cy;

	int i;

	for(i = 0; i < 2048; i++)
	{
		float x = (zx * zx - zy * zy) + cx;
		float y = (zy * zx + zx * zy) + cy;

		if((x * x + y * y) > 4.0) break;
		zx = x;
		zy = y;
	}


	output.Pos.z = 1.0 - float(i)/10000.0;
	
	output.Pos = mul( output.Pos, World );
	output.Pos = mul( output.Pos, View );
	output.Pos = mul( output.Pos, Projection );
	return output;
}

[maxvertexcount(3)]   // produce a maximum of 3 output vertices
void GS( triangle PS_INPUT input[3], inout TriangleStream<PS_INPUT> triStream )
{
	PS_INPUT psInput;

	float3 faceEdgeA = input[1].Pos - input[0].Pos;
	float3 faceEdgeB = input[2].Pos - input[0].Pos;
	float3 faceNormal = normalize( cross(faceEdgeA, faceEdgeB) );

	psInput.Pos = input[0].Pos;
	psInput.Norm = faceNormal;
	triStream.Append(psInput);

	psInput.Pos = input[1].Pos;
	psInput.Norm = faceNormal;
	triStream.Append(psInput);

	psInput.Pos = input[2].Pos;
	psInput.Norm = faceNormal;
	triStream.Append(psInput);

	triStream.RestartStrip();
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
	static float3 vLightDir = float3( 0.0f, 0.0f, -1.0f);
	static float3 vLightColor = float3( 1.0f, 1.0f, 1.0f);

	float3 finalColor = 0;
    
	finalColor += saturate( dot( vLightDir,input.Norm) * vLightColor );

	return float4(finalColor[0], finalColor[1], finalColor[2], 1.0f);
}
