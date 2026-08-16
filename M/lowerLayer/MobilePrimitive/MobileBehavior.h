#pragma once
#include "../../M.h"
#include "../../../lowerLayer/engineCore/Mesh/meshStructure/Vertex/Vertex.h"
#include "../../../lowerLayer/engineCore/Render/Appearance/Appearance.h"

struct MobileBehavior
{
	Appearance appearance;
	DrawMode drawMode;

	virtual void Update() = 0;
	virtual void Draw(Matrix4* vpMat_) = 0;

	MobileBehavior();
};

