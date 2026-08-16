#pragma once
#include "../../../Mesh/meshStructure/Vertex/Vertex.h"
#include "../StructuredBufferAndMap.h"

struct TrinangleIndices
{
	UINT slot0 = {};
	UINT slot1 = {};
	UINT slot2 = {};
};


struct IndexStructuredBuffer 
{
	StructuredBufferAndMap<uint32_t> indices;
};


