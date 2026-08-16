#pragma once
#include "../../math/vector/vector.h"
#include <functional>

struct Rect;

//なにこれ？
struct CollisionMapInfo
{
	//天井にぶつかっているか
	bool isCeiling = false;
	//接地判定
	bool isGround = false;
	//壁とぶつかったか
	bool isWall = false;
	//動くブロックの上に載っているか
	bool isOnMovableBlock = false;

	Vector3 deltaPos = { 0.0f,0.0f,0.0f };
};

namespace CollisionDetections
{
	namespace C2D
	{
		bool SegmentAndCircle(float radius_, Vector2 circlePos_, Vector2 segStart_, Vector2 segEnd_);
		//2つのオブジェクトが衝突したとき、thisObjのポジションを補正する（めり込み補正）
		bool FixThisObjPosWhenCollided(Vector3* thisPos_, Vector3 thisPrePos_,
			float thisWidth_, float thisHeight_, Vector3& thisVelocity_, CollisionMapInfo& colMapInfo_,
			Vector3* otherPos_, Vector3 otherPrePos_,
			float otherWidth_, float otherHeight_,bool senyou_ = false);
		bool CircleCollision(float lRad_, Vector3 lPos_, float rRad_, Vector3 rPos_);
		bool ObjectAABB(Rect* thisRect_, Vector3 thisWorld_,
			Rect* otherRect_, Vector3 otherWorld_);

	}

	namespace C3D
	{

	}


};

