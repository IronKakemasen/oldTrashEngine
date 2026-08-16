#pragma once

#include "../../../utilities/Transform/Transform.h"
#include "../../../utilities/UVTransform/UVTransform.h"
#include "../../M.h"
#include "../../../lowerLayer/engineCore/Mesh/meshStructure/Vertex/Vertex.h"

struct ParticleBehavior2D
{
public:
	virtual void Update() = 0;
	virtual void Init() = 0;
	virtual void Draw(Matrix4* vpMat_)= 0;

	ParticleBehavior2D();

	std::vector<Transform> trans;
	UVTransform uvTrans;

protected:
	int numParticle;

protected:
	void SetNumParticle(int num_);
	inline auto Watch_Numparticles()
	{
		return numParticle;
	}

};
