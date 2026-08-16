#pragma once
#include "../MobileBehavior.h"

struct MobileQuadBehavior:MobileBehavior
{
	Vertex leftTop;
	Vertex rightTop;
	Vertex leftBottom;
	Vertex rightBottom;

	static inline int count = -1;

	virtual void Update() = 0;
	virtual void Draw(Matrix4* vpMat_)override {};

	MobileQuadBehavior();

	void Initialize(float width_, float height_, Vector3 world_pos_, int texHandle_, Vector4 color_ = { 255,255,255,255 });
	void Initialize(Vector3 local_leftTop_, Vector3 local_rightTop_, Vector3 local_leftBottom_, Vector3 local_rightBottom_,
		Vector3 world_pos_, int texHandle_, Vector4 color_ = { 255,255,255,255 });

	//頂点設定
	void SetNormal();
	//void SetTexcoord(Vector2 leftTop_ = { 0.0f,0.0f }, Vector2 rightTop_ = { 1.0f,0.0f }, 
	//	Vector2 rightBottom_ = { 1.0f,1.0f }, Vector2 leftBottom_ = {0.0f,1.0f});

	void SetTexcoord(Vector2 leftTop_ = { -0.5f,-0.5f }, Vector2 rightTop_ = { 0.5f,-0.5f },
		Vector2 rightBottom_ = { 0.5f,0.5f }, Vector2 leftBottom_ = { -0.5f,0.5f });

	void SetVertex(float width_, float height_);
	void SetVertex(Vector3 local_leftTop_, Vector3 local_rightTop_, Vector3 local_rightBottom_, Vector3 local_leftBottom_);
	void ResetTexcoord();

};

