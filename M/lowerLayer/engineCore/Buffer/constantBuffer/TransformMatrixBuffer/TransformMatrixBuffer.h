#pragma once
#include "../bufferAndMap.h"
#include "../../../Mesh/meshStructure/TransformMatrix/TransformMatrix.h"

struct TransformationParam
{
    //Matrix4 view;
    //Matrix4 proj;
    Matrix4 wvp;
    Matrix4 world;

};

struct TransformMatrixBuffer
{
	BufferAndMap<TransformMatrix> matrix;
};

struct TransformMatrixBuffer2
{
    BufferAndMap<TransformationParam> matrix;
};




