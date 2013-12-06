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
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = input.Pos;
	return output;
}

[maxvertexcount(2)]   // produce a maximum of 3 output vertices
void GS( point PS_INPUT input[1], inout PointStream<PS_INPUT> pntStream )
{
	PS_INPUT psInput = (PS_INPUT)0;

	psInput.Pos = input[0].Pos;

	psInput.Pos = mul( psInput.Pos, World );
	psInput.Pos = mul( psInput.Pos, View );
	psInput.Pos = mul( psInput.Pos, Projection );
	
	pntStream.Append(psInput);


	pntStream.RestartStrip();
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
