#include "MQuad.h"

void MQuad::Update()
{

}

void MQuad::Draw(Matrix4* vpMat_)
{
	M::GetInstance()->DrawMQuad(leftTop, rightTop, rightBottom,leftBottom,
		appearance.color, appearance.use_texHandles[0], drawMode, appearance.blendMode,
		appearance.cullMode, appearance.shaderSetIndex,appearance.trans, appearance.uvTrans, *vpMat_);
}
