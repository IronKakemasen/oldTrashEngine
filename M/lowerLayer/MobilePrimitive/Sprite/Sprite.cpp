#include "Sprite.h"

void Sprite::ResetTexcoord()
{
	leftTop.texcoord = { 0.0f,0.0f };
	rightTop.texcoord = { 1.0f,0.0f };
	rightBottom.texcoord = { 1.0f,1.0f };
	leftBottom.texcoord = { 0.0f,1.0f };
	appearance.uvTrans.pos = { 0.5f,0.5f };
}

Appearance* Sprite::GetAppearance()
{
	return &appearance;
}

Sprite::Sprite()
{
	Sprite::count++;
	appearance.shaderSetIndex = M::GetInstance()->
		GetShaderSetIndexFromFileName("Sprite.VS", "Sprite.PS");

}

void Sprite::Initialize(float width_, float height_, Vector3 world_pos_, int texHandle_, Vector4 color_)
{
	SetVertex(width_, height_);
	SetTexcoord();
	appearance.trans.pos = world_pos_;
	appearance.color = color_;
	appearance.use_texHandles[0] = (int)texHandle_;
}

void Sprite::Initialize(Vector3 local_leftTop_, Vector3 local_rightTop_, Vector3 local_leftBottom_, Vector3 local_rightBottom_,
	Vector3 world_pos_, int texHandle_, Vector4 color_)
{
	SetVertex(local_leftTop_, local_rightTop_, local_rightBottom_, local_leftBottom_);
	SetTexcoord();
	appearance.trans.pos = world_pos_;
	appearance.color = color_;
	appearance.use_texHandles[0] = (int)texHandle_;
}


void Sprite::SetVertex(Vector3 local_leftTop_, Vector3 local_rightTop_, Vector3 local_rightBottom_, Vector3 local_leftBottom_)
{
	leftTop.position = local_leftTop_;
	rightTop.position = local_rightTop_;
	rightBottom.position = local_rightBottom_;
	leftBottom.position = local_leftBottom_;
}
void Sprite::SetVertex(float width_, float height_)
{
	//左上の頂点
	leftTop.position.x = -width_ * 0.5f;
	leftTop.position.y = -height_ * 0.5f;
	leftTop.position.z = 0.0f;
	//右上の頂点
	rightTop.position.x = width_ * 0.5f;
	rightTop.position.y = -height_ * 0.5f;
	rightTop.position.z = 0.0f;

	//左下の頂点
	leftBottom.position.x = -width_ * 0.5f;
	leftBottom.position.y = height_ * 0.5f;
	leftBottom.position.z = 0.0f;

	//右下の頂点
	rightBottom.position.x = width_ * 0.5f;
	rightBottom.position.y = height_ * 0.5f;
	rightBottom.position.z = 0.0f;

}

void Sprite::ToAtlas(float numTexture_)
{
	if (!atlas) atlas.reset(new Atlas);

	atlas->uvCoordinateRate = 1.0f / numTexture_;
	float const half = atlas->uvCoordinateRate * 0.5f;

	SetTexcoord({ -half,-0.5f }, { half,-0.5f }, { half,0.5f }, { -half,0.5f });
	appearance.uvTrans.pos = { half,0.5f };

}

void Sprite::ChangeAtlasIndex(float index_)
{
	if (!atlas) return;
	atlas->ChangeIndex(index_);
}


void Sprite::SetTexcoord(Vector2 leftTop_, Vector2 rightTop_, Vector2 rightBottom_, Vector2 leftBottom_)
{
	leftTop.texcoord = leftTop_;
	rightTop.texcoord = rightTop_;
	rightBottom.texcoord = rightBottom_;
	leftBottom.texcoord = leftBottom_;
}

void Sprite::Update()
{
	if(atlas)
	{
		appearance.uvTrans.pos.x = atlas->CalcUvPosX();
	}

}

void Sprite::Draw(Matrix4* ortho_)
{
	M::GetInstance()->DrawSprite(leftTop, rightTop, rightBottom, leftBottom,
		appearance.color, appearance.use_texHandles[0], drawMode, appearance.blendMode,
		appearance.cullMode, appearance.shaderSetIndex, appearance.trans, appearance.uvTrans, *ortho_);
}
