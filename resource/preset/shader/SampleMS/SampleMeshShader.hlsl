struct MSInput
{
    float4 position;
    float4 normal;
    float4 tangent;
    float4 texcoord;
    
};

struct MSOutput
{
    float4 position : SV_Position;
    float4 color : TEXCOORD0;
};

struct TransformationParam
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
    float4x4 wvp;
    
};

struct Sigh
{
    float4x4 wvp;
};

StructuredBuffer<MSInput> g_vertices : register(t0);
StructuredBuffer<uint> g_indices : register(t1);
//ConstantBuffer<TransformationParam> g_transform : register(b0);
ConstantBuffer<Sigh> g_transform : register(b0);


[numthreads(64, 1, 1)]

[outputtopology("triangle")]

void main
( 
    uint groupIndex_ : SV_GroupIndex,
    out vertices MSOutput verts_[3],    //出力される頂点データ
    out indices uint3 tris_[1]          //出力される頂点番号
)
{
    //スレッドグループの頂点数とポリゴン数を設定
    SetMeshOutputCounts(3, 1);
    
    //頂点番号を設定
    if (groupIndex_< 1 )
    {
        tris_[groupIndex_] = uint3(g_indices[0], g_indices[1], g_indices[2]);
    }
    
    if (groupIndex_ < 3)
    {
        uint vertexIndex = g_indices[groupIndex_];
                
        MSOutput output;
        
        float4 localPos = float4(g_vertices[vertexIndex].position.xyz, 1.0f);
        //float3 worldPos = mul(localPos , g_transform.world).xyz;
        
        output.position = mul(localPos , g_transform.wvp);
        output.color = float4(1.0f, 0.0f, 0.0f, 1.0f);
        
        verts_[groupIndex_] = output;
    }
        
}