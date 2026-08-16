#pragma once
#include "../PrimitiveMeshCommon.h"

struct QuadMesh :PrimitiveMeshCommon
{
	virtual void CreateMesh(ID3D12Device* device_) override;
	QuadMesh(AllPipelineSet* allPipelineset_);
};