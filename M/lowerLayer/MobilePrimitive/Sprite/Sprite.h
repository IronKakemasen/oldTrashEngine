#pragma once
#include "../MobileBehavior.h"
#include "Atlas.h"

class Sprite :MobileBehavior
{
	std::unique_ptr<Atlas> atlas;

	static inline int count = -1;
	void SetVertex(float width_, float height_);
	void SetVertex(Vector3 local_leftTop_, Vector3 local_rightTop_, Vector3 local_rightBottom_, Vector3 local_leftBottom_);

public:

	Vertex leftTop;
	Vertex rightTop;
	Vertex leftBottom;
	Vertex rightBottom;

	virtual void Update()override;
	virtual void Draw(Matrix4* ortho_)override;
	void ToAtlas(float numTexture_);
	void ChangeAtlasIndex(float index_);
	Sprite();

	void Initialize(float width_, float height_, Vector3 world_pos_, int texHandle_, Vector4 color_ = { 255,255,255,255 });
	void Initialize(Vector3 local_leftTop_, Vector3 local_rightTop_, Vector3 local_leftBottom_, Vector3 local_rightBottom_,
		Vector3 world_pos_, int texHandle_, Vector4 color_ = { 255,255,255,255 });

	void SetTexcoord(Vector2 leftTop_ = { -0.5f,-0.5f }, Vector2 rightTop_ = { 0.5f,-0.5f },
		Vector2 rightBottom_ = { 0.5f,0.5f }, Vector2 leftBottom_ = { -0.5f,0.5f });

	void ResetTexcoord();
	Appearance* GetAppearance();

};

