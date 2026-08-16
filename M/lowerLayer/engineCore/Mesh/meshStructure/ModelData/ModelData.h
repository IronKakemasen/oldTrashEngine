#pragma once
#include "../../meshStructure/Vertex/Vertex.h"
#include "../../../Render/Appearance/Appearance.h"
#include "ResMeshlet.h"
#include <vector>
#include <string>
#include "../../../Buffer/StructuredBuffer/StructuredBufferAndMap.h"

struct Vertex2
{
    Vector4 position;
    Vector4 normal;
    Vector4 tangent;
    Vector4 texcoord;
};

struct VertexStructuredBuffer
{
    StructuredBufferAndMap<Vertex2> vertex;
};


struct ST4TestMeshShader
{
    StructuredBufferAndMap<uint32_t> uniqueVertexIndicesSB; // 1
    StructuredBufferAndMap<ResPrimitiveIndex> primitiveIndicesSB; // 1
    StructuredBufferAndMap<ResMeshlet> meshletSB; // メッシュレットの個数分
};


struct ResMaterial
{
    Vector3 diffuse;   
    Vector3 specular;
    float alpha{};
    float shininess{};
    std::wstring colorMap;    
    std::wstring specularMap; 
    std::wstring shininessMap; 
    std::wstring normalMap;   
};

struct ResMesh
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
    uint32_t materialId;   

    //メッシュレット
    std::vector<ResMeshlet> meshlets;
    //ユニーク頂点インデックス(threadGroupの総数でもある)
    std::vector<uint32_t> uniqueVertexIndices;
    //プリミティブインデックス
    std::vector<ResPrimitiveIndex> primitiveIndices;

    StructuredBufferAndMap<Vertex2> vertexData;
    StructuredBufferAndMap<uint32_t> uniqueVertexIndexData;
    StructuredBufferAndMap<ResMeshlet> meshletData;
    StructuredBufferAndMap<ResPrimitiveIndex> primitiveIndicesData;

    
};

struct ModelData
{
    std::vector < ResMaterial > resMaterial;
    std::vector < ResMesh > resMesh;
};
