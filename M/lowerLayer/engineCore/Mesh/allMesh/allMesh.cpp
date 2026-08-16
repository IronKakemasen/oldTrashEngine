#include "allMesh.h"

void AllMesh::ResetDrawIndexes()
{
	triangleMesh->DrawIndexReset();
	quadMesh->DrawIndexReset();
	lineMesh->DrawIndexReset();
	meshForPostEffect->curIndex = 0;
}

void AllMesh::Init(ID3D12Device* device_, ParticleMeshSrvCreator* ParticleMeshSrvCreator_, AllPipelineSet* allPipelineSet_)
{
	static bool initOnlyOnce = true;

	if (initOnlyOnce)
	{
		initOnlyOnce = false;

		triangleMesh.reset(new TriangleMesh(allPipelineSet_));
		quadMesh.reset(new QuadMesh(allPipelineSet_));
		testParticleMesh.reset(new TestParticleMesh(allPipelineSet_));
		lineMesh.reset(new LineMesh(allPipelineSet_,device_));
		meshForPostEffect.reset(new MeshForPostEffect(device_, allPipelineSet_));

		triangleMesh->CreateMesh(device_);
		quadMesh->CreateMesh(device_);
		testParticleMesh->CreateMesh(device_,ParticleMeshSrvCreator_);
	}
}