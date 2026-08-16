#pragma once
#include "../bufferAndMap.h"
#include "../../../Mesh/meshStructure/material/material.h"
#include "../../../Mesh/meshStructure/MaterialForLine/MaterialForLine.h"

struct MaterialBuffer
{
	BufferAndMap < Material > material;
};

struct MaterialForLineBuffer
{
	BufferAndMap < MaterialForLine > material;
};

