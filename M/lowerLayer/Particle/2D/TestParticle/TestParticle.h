#pragma once
#include "../ParticleBehavior2D.h"

class TestParticle:ParticleBehavior2D
{
public:
	TestParticle();
	virtual void Update()override;
	virtual void Draw(Matrix4* vpMat_)override;
	virtual void Init()override;

};

