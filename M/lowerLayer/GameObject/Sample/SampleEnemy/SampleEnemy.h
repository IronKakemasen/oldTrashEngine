#pragma once
#include "../../GameObjectBehavior.h"
#include "../../../MobilePrimitive/MobileTriangle/MTriangle/MTriangle.h"

class SampleEnemy:GameObject
{
	MTriangle triangleModel;

public:
	virtual void Update()override;
	virtual void Init() override;
	virtual void Reset() override;
	virtual void Draw(Matrix4* vpMat_)override;
	virtual void SetCollisionBackTable()override;

	SampleEnemy();
};

