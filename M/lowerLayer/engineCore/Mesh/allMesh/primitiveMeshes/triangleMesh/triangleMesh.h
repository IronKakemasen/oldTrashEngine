#include "../PrimitiveMeshCommon.h"


struct TriangleMesh:PrimitiveMeshCommon
{
	virtual void CreateMesh(ID3D12Device* device_) override;

	TriangleMesh(AllPipelineSet* allPipelineset_);
};