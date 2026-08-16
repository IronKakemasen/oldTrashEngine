#pragma once
#include "../ParticleMesh2DCommon.h"

struct TestParticleMesh:ParticleMesh2DCommon
{
public:
	virtual void CreateMesh(ID3D12Device* device_, ParticleMeshSrvCreator* particleMeshSrvCreator_) override;

	TestParticleMesh(AllPipelineSet* allPipelineset_);
};

