#include "../HLSLI/CameraPara.hlsli"

struct VetexInput
{
    float4 position;
    float4 normal;
    float4 tangent;
    float4 texcoord;
};

struct MSOutput
{
    float4 position : SV_Position;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : POSITOIN0;

};

struct Meshlet
{
	//頂点番号オフセット
    uint vertexOffset;
	//頂点数
    uint vertexCnt;
	//プリミティブ番号オフセット
    uint primitiveOffset;
	//プリミティブオフセット
    uint primitiveCnt;
};

struct WVP
{
    float4x4 wvp;
};


struct Kusoga
{
    float4x4 wvp;
    float4x4 world;
};

uint3 UnpackPrimitiveIndex(uint packedIndex_)
{
    //0x3ff = 0b1111111111
    //>>10 = 10bit右側にずらす
    return uint3(
        packedIndex_ & 0x3FF,
        (packedIndex_ >> 10) & 0x3FF,
        (packedIndex_ >> 20) & 0x3FF);
}


StructuredBuffer<VetexInput> vertices : register(t0);
StructuredBuffer<uint> uniqueIndices : register(t1);
StructuredBuffer<Meshlet> meshlets : register(t2);
StructuredBuffer<uint> primitiveIndices : register(t3);

ConstantBuffer<Kusoga> transform : register(b0);

[numthreads(64,1,1)]
[outputtopology("triangle")]
void main
(
    uint groupThreadID_ : SV_GroupThreadID,
    uint groupID_ : SV_GroupID,
    out vertices MSOutput verts_[64],
    out indices uint3 polys_[126]
)
{
    Meshlet meshlet = meshlets[groupID_];
    
    //スレッドグループの頂点数とポリゴン数を設定
    SetMeshOutputCounts(meshlet.vertexCnt, meshlet.primitiveCnt);
    
    if (groupThreadID_ < meshlet.vertexCnt)
    {
        uint accessID_uniqueIndices = groupThreadID_ + meshlet.vertexOffset;
        uint vertexIndex = uniqueIndices[accessID_uniqueIndices];
        VetexInput dst_vertex = vertices[vertexIndex];

        float4 localPos = float4(dst_vertex.position.xyz, 1.0f);
        verts_[groupThreadID_].position = mul(localPos, transform.wvp);
        verts_[groupThreadID_].normal = normalize(mul(dst_vertex.normal, transform.world).xyz);
        verts_[groupThreadID_].texcoord = dst_vertex.texcoord.xy;

    }
    
    
    for (uint i = groupThreadID_; i < meshlet.primitiveCnt; i += 64)
    {
        uint accessID_primitiveIndices = meshlet.primitiveOffset + i;
        uint packedIndex = primitiveIndices[accessID_primitiveIndices];    
        polys_[i] = UnpackPrimitiveIndex(packedIndex);
    }
        
}