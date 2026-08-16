#pragma once
#include "../../GameObjectBehavior.h"
#include "../../../MobilePrimitive/MobileQuad/MQuad/MQuad.h"

class SamplePlayer :public GameObject
{
	MQuad quadModel;
	Vector3 velocity;

public:
	virtual void Update()override;
	virtual void Init() override;
	virtual void Reset() override;
	virtual void Draw(Matrix4* vpMat_)override;
	virtual void SetCollisionBackTable()override;

	SamplePlayer();
};

