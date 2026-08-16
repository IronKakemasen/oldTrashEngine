#pragma once
#include "../../M.h"
#include "../../../lowerLayer/engineCore/Mesh/meshStructure/Vertex/Vertex.h"
#include "../../../lowerLayer/engineCore/Render/Appearance/Appearance.h"
#include "../../../lowerLayer/engineCore/Mesh/allMesh/MeshForModel/ModelSimple/ModelSimple.h"
#include "../../M/utilities/Counter/Counter.h"
#include "../../M/utilities/Easing/EasingFunctions.h"

struct ModelBehavior
{
	std::unique_ptr<ModelSimple> model;

	ModelBehavior();

	virtual void Update(int mode_ = 0,float count_ = 0.0f) = 0;
	virtual void Draw(Matrix4* vpMat_) = 0;
	virtual void Init(Transform* gameObjectTrans_) = 0;
	virtual void Reset() = 0;

	void MakeAllPartsBeChildren(Transform* gameObject_);

};

