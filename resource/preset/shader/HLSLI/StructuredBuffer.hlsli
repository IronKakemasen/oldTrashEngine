#pragma once

struct StandardVertex
{
    float4 localPos;
    float4 normal;
    float4 tangent;
    float4 texcoord;
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

struct OriginalIndex
{
    uint index;
};

struct PrimitiveIndex
{
    uint packedPrimitiveIndex;
};

struct UniqueVertexIndex
{
    uint index;
};

struct TransformMatrix
{
    float4x4 wvp;
    float4x4 world;
};

struct MeshDataSRVHeapIndexGroup
{
    uint vertices;
    uint uniqueVertexIndices;
    uint meshlets;
    uint primitiveIndices;

};
