#include "../HLSLI/TransformMatrix.hlsli"
#include "../HLSLI/VertexShaderInput.hlsli"


struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

ConstantBuffer<WVPMatrix> gWVPMatrix : register(b0);

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	output.texcoord = input.texcoord;
    float4 tmp = float4(input.position.x, input.position.y, input.position.z,1.0f);
    
    output.position = mul(tmp, gWVPMatrix.WVP);
    
	return output;
}



