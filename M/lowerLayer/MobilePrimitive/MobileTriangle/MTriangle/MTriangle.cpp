#include "MTriangle.h"

void MTriangle::Update()
{

}

void MTriangle::Draw(Matrix4* vpMat_)
{
	M::GetInstance()->DrawMTriangle(left, top, right, appearance.color, 
		appearance.use_texHandles[0], drawMode, appearance.blendMode, appearance.cullMode,
		appearance.shaderSetIndex,appearance.trans, appearance.uvTrans, *vpMat_);
}
