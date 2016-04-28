cbuffer ConstantBuffer
{
	float4x4 MatProjection;
	float4x4 rotationMatrix;
}

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float light : LIGHT;
};

VOut VertexasdShader(float4 position : POSITION, float4 normal : NORMAL, float4 color : COLOR)
{
    VOut output;

    output.position = mul(MatProjection, position);
    //output.position = position;
    output.color = color;

	float4 globalLightDir = {-1.0f, 1.0f, -1.0f, 0.0f};
	float4 rotatedNormal = normalize(mul(rotationMatrix, normal));
    output.light = saturate(dot(rotatedNormal, globalLightDir));

    return output;
}


float4 PixelasdShader(float4 position : SV_POSITION, float4 color : COLOR, float light : LIGHT) : SV_TARGET
{
	float4 ambientColor = {0.2f, 0.2f, 0.2f, 1.0f};

	if (light < 0.2) {
		light = 0.2f;
	}

	float4 outputColor = color * light;
    return outputColor;
}