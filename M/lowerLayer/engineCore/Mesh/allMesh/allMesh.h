#pragma once
#include "./primitiveMeshes/triangleMesh/triangleMesh.h"
#include "./primitiveMeshes/quadMesh/quadMesh.h"
#include "./ParticleMesh2D/TestParticleMesh/TestParticleMesh.h"
#include "./LineMesh/LineMesh.h"
#include "./MeshForPostEffect/MeshForPostEffect.h"

class ParticleMeshSrvCreator;
class AllPipelineSet;

class AllMesh
{
	std::unique_ptr<TriangleMesh> triangleMesh;
	std::unique_ptr<QuadMesh> quadMesh;
	std::unique_ptr<TestParticleMesh> testParticleMesh;
	std::unique_ptr<LineMesh> lineMesh;
	std::unique_ptr<MeshForPostEffect> meshForPostEffect;

	
public:

	void ResetDrawIndexes();
	void Init(ID3D12Device* device_, ParticleMeshSrvCreator* ParticleMeshSrvCreator_, AllPipelineSet* allPipelineSet_);

	inline auto* Getter_TestParticleMesh()
	{
		return testParticleMesh.get();
	}

	inline auto* Getter_TriangleMesh()
	{
		return triangleMesh.get();
	}

	inline auto* Getter_LineMesh()
	{
		return lineMesh.get();
	}

	inline auto* Getter_QuadMesh()
	{
		return quadMesh.get();
	}

	inline auto* Getter_MeshForPostEffect()
	{
		return meshForPostEffect.get();
	}

};

