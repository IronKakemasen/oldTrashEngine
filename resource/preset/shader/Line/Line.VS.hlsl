#include "../HLSLI/TransformMatrix.hlsli"
#include "../HLSLI/VertexShaderInput.hlsli"

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float3 depthInView : TEXCOORD1;

};

ConstantBuffer<WVPMatrix> viewProjectiton : register(b0);

VertexShaderOutput main(VertexShaderInputLine input)
{
	VertexShaderOutput output;
    float4 tmp = float4(input.position.x, input.position.y, input.position.z,1.0f);
    output.position = mul(tmp, viewProjectiton.WVP);
    
    output.depthInView = float3(0, 0, 0);
    
	return output;
}



