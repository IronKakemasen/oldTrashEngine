#pragma once
#include "ModelBehavior.h"


class AxisModel :public ModelBehavior
{

public:
	AxisModel();
	virtual void Update(int mode_ = 0, float count_ = 0.0f)override;
	virtual void Draw(Matrix4* vpMat_)override;
	virtual void Init(Transform* gameObjectTrans_)override;
	virtual void Reset()override;

};

