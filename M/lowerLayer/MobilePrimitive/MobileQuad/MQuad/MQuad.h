#pragma once
#include "../MobileQuadBehavior.h"

struct MQuad:MobileQuadBehavior
{
	virtual void Update()override;
	virtual void Draw(Matrix4* vpMat_)override;
};

