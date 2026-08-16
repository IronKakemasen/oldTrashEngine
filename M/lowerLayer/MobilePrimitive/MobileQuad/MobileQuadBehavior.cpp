#include "MobileQuadBehavior.h"

void MobileQuadBehavior::ResetTexcoord()
{
	leftTop.texcoord = { 0.0f,0.0f };
	rightTop.texcoord = { 1.0f,0.0f };
	rightBottom.texcoord = { 1.0f,1.0f };
	leftBottom.texcoord = { 0.0f,1.0f };
	appearance.uvTrans.pos = { 0.5f,0.5f };
}

MobileQuadBehavior::MobileQuadBehavior()
{
	MobileQuadBehavior::count++;

}

void MobileQuadBehavior::Initialize(float width_, float height_, Vector3 world_pos_, int texHandle_, Vector4 color_)
{
	SetVertex(width_, height_);
	SetNormal();
	SetTexcoord();
	appearance.trans.pos = world_pos_;
	appearance.color = color_;
	appearance.use_texHandles[0] = (int)texHandle_;
	appearance.shaderSetIndex = M::GetInstance()->
		GetShaderSetIndexFromFileName("MobilePrimitiveMesh.VS", "MobilePrimitiveMesh.PS");

}

void MobileQuadBehavior::Initialize(Vector3 local_leftTop_, Vector3 local_rightTop_, Vector3 local_leftBottom_, Vector3 local_rightBottom_,
	Vector3 world_pos_, int texHandle_, Vector4 color_)
{
	SetVertex(local_leftTop_, local_rightTop_, local_rightBottom_, local_leftBottom_);
	SetNormal();
	SetTexcoord();
	appearance.trans.pos = world_pos_;
	appearance.color = color_;
	appearance.use_texHandles[0] = (int)texHandle_;
	appearance.shaderSetIndex = M::GetInstance()->
		GetShaderSetIndexFromFileName("MobilePrimitiveMesh.VS", "MobilePrimitiveMesh.PS");

}


void MobileQuadBehavior::SetVertex(Vector3 local_leftTop_, Vector3 local_rightTop_, Vector3 local_rightBottom_, Vector3 local_leftBottom_)
{
	leftTop.position = local_leftTop_;
	rightTop.position = local_rightTop_;
	rightBottom.position = local_rightBottom_;
	leftBottom.position = local_leftBottom_;
}
void MobileQuadBehavior::SetVertex(float width_, float height_)
{
	//左上の頂点
	leftTop.position.x = -width_ * 0.5f;
	leftTop.position.y = height_ * 0.5f;
	leftTop.position.z = 0.0f;
	//右上の頂点
	rightTop.position.x = width_ * 0.5f;
	rightTop.position.y = height_ * 0.5f;
	rightTop.position.z = 0.0f;

	//左下の頂点
	leftBottom.position.x = -width_ * 0.5f;
	leftBottom.position.y = -height_ * 0.5f;
	leftBottom.position.z = 0.0f;

	//右下の頂点
	rightBottom.position.x = width_ * 0.5f;
	rightBottom.position.y = -height_ * 0.5f;
	rightBottom.position.z = 0.0f;

}


void MobileQuadBehavior::SetNormal()
{
	Vector3 LB2LT = leftTop.position - leftBottom.position;		//↑
	Vector3 LT2RT = rightTop.position - leftTop.position;		//→
	Vector3 RT2RB = rightBottom.position - rightTop.position;	//↓
	Vector3 RB2LB = leftBottom.position - rightBottom.position;	//←

	Vector3 crossVec;
	crossVec = LB2LT.GetCross(LT2RT);
	leftTop.normal = { crossVec.x,crossVec.y,crossVec.z };

	crossVec = LT2RT.GetCross(RT2RB);
	rightTop.normal = { crossVec.x,crossVec.y,crossVec.z };

	crossVec = RT2RB.GetCross(RB2LB);
	rightBottom.normal = { crossVec.x,crossVec.y,crossVec.z };

	crossVec = RB2LB.GetCross(LB2LT);
	leftBottom.normal = { crossVec.x,crossVec.y,crossVec.z };

}

void MobileQuadBehavior::SetTexcoord(Vector2 leftTop_, Vector2 rightTop_, Vector2 rightBottom_, Vector2 leftBottom_)
{
	leftTop.texcoord = leftTop_;
	rightTop.texcoord = rightTop_;
	rightBottom.texcoord = rightBottom_;
	leftBottom.texcoord = leftBottom_;
}


