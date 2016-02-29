cbuffer ConstantBuffer
{
	float4x4 MatProjection;
}

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VOut VertexasdShader(float4 position : POSITION, float4 color : COLOR)
{
    VOut output;

    output.position = mul(position, MatProjection);
    //output.position = position;
    output.color = color;

    return output;
}


float4 PixelasdShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}