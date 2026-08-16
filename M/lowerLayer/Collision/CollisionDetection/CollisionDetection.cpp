#include "CollisionDetection.h"
#include "../Shape/Rect.h"

//2D-----------------------------------------------------------------------------------------------------------------------------------
bool CollisionDetections::C2D::SegmentAndCircle(float radius_, Vector2 circlePos_, Vector2 segStart_, Vector2 segEnd_)
{
	bool ret = false;

	Vector2 seg = segEnd_ - segStart_;
	Vector2 s2c = circlePos_ - segStart_;
	Vector2 e2c = circlePos_ - segEnd_;
	Vector2 e_seg = seg.GetNormalized();

	float dot = s2c.GetDot(seg);
	float inv_mag = 1.0f / (seg.GetMagnitutde() * seg.GetMagnitutde());
	Vector2 proj = seg * inv_mag * dot;
	Vector2 nearPoint = { segStart_.x + proj.x,segStart_.y + proj.y };
	float length = (circlePos_ - nearPoint).GetMagnitutde();

	if (length < radius_)
	{
		float cos1 = s2c.GetCos(seg);
		float cos2 = e2c.GetCos(seg);

		// 二つの内積の掛け算結果が0以下なら当たり
		if (cos1 * cos2 <= 0.0f)
		{
			ret = true;

		}

		if (s2c.GetMagnitutde() < radius_ || e2c.GetMagnitutde() < radius_)
		{
			ret = true;
		}
	}

	return ret;
}


bool CollisionDetections::C2D::ObjectAABB(Rect* thisRect_,Vector3 thisWorld_ ,
	Rect* const otherRect_, Vector3 otherWorld_)
{
	Rect t = thisRect_->ConvertToWorld(thisWorld_);
	Rect o = otherRect_->ConvertToWorld(otherWorld_);

	bool ret = false;

	if ((t.left < o.right) && (t.right > o.left))
	{
		if ((t.bottom < o.top) && (t.top > o.bottom))
		{
			ret = true;
		}
	}

	return ret;
}

bool CollisionDetections::C2D::CircleCollision(float lRad_, Vector3 lPos_, float rRad_, Vector3 rPos_)
{
	Vector3 diff = lPos_ - rPos_;
	if (!diff.IsBigger(lRad_ + rRad_))return true;

	return false;
}

//2つのオブジェクトが衝突したとき、thisObjのポジションを補正する（めり込み補正）
bool CollisionDetections::C2D::FixThisObjPosWhenCollided(Vector3* thisPos_, Vector3 thisPrePos_,
	float thisWidth_, float thisHeight_, Vector3& thisVelocity_, CollisionMapInfo& colMapInfo_,
	Vector3* otherPos_, Vector3 otherPrePos_,
	float otherWidth_, float otherHeight_, bool senyou_)
{
	bool ret = false;

	////移動前の矩形
	//Rect pre_thisRect;
	//pre_thisRect.SetShape(thisPrePos_, thisWidth_, thisHeight_);
	//Rect pre_otherRect;
	//pre_otherRect.SetShape(otherPrePos_, otherWidth_, otherHeight_);

	////移動後の矩形
	//Rect thisRect;
	//thisRect.SetShape(*thisPos_, thisWidth_, thisHeight_);
	//Rect otherRect;
	//otherRect.SetShape(*otherPos_, otherWidth_, otherHeight_);

	//if (ObjectAABB(&thisRect, &otherRect))
	//{
	//	//右からぶつかる
	//	if (pre_thisRect.left >= pre_otherRect.right)
	//	{
	//		if (pre_thisRect.bottom < otherRect.top && pre_thisRect.bottom > otherRect.bottom - otherWidth_)
	//		{
	//			float delta = otherRect.right - thisRect.left;

	//			if (delta >= 0)
	//			{
	//				thisPos_->x += delta + delta * 0.01f;
	//				//改めてプレイヤーの四つ角せってい
	//				thisRect.SetShape(*thisPos_, thisWidth_, thisHeight_);

	//				ret = true;
	//				colMapInfo_.isWall = true;
	//			}
	//		}
	//	}

	//	//左からぶつかる
	//	if (pre_thisRect.right <= pre_otherRect.left)
	//	{
	//		if (pre_thisRect.bottom < otherRect.top && pre_thisRect.bottom > otherRect.bottom - otherHeight_)
	//		{
	//			float delta = thisRect.right - otherRect.left;

	//			if (delta >= 0)
	//			{
	//				thisPos_->x -= delta + delta * 0.01f;
	//				//改めてプレイヤーの四つ角せってい
	//				thisRect.SetShape(*thisPos_, thisWidth_, thisHeight_);

	//				ret = true;
	//				colMapInfo_.isWall = true;

	//			}
	//		}
	//	}
	//	else
	//	{

	//	}

	//	if (
	//		(thisVelocity_.x == 0) ||
	//		(thisVelocity_.x <0 && pre_thisRect.left > otherRect.left - otherWidth_ && pre_thisRect.left < otherRect.right) ||
	//		(thisVelocity_.x > 0 && pre_thisRect.right < otherRect.right + otherWidth_ && pre_thisRect.right > otherRect.left)

	//		)
	//	{
	//		//プレイヤーがオブジェクトの上にいた場合
	//		if (pre_thisRect.bottom >= pre_otherRect.top)
	//		{
	//			float delta = otherRect.top - thisRect.bottom;
	//			if (delta >= 0)
	//			{
	//				thisVelocity_.y = 0.0f;

	//				thisPos_->y += delta + delta * 0.001f;
	//				//改めてプレイヤーの四つ角せってい
	//				thisRect.SetShape(*thisPos_, thisWidth_, thisHeight_);
	//				ret = true;

	//				colMapInfo_.isGround = true;

	//				if (senyou_)
	//				{
	//					colMapInfo_.isOnMovableBlock = true;
	//				}

	//			}
	//		}

	//		//プレイヤーがオブジェクトの下にいた場合
	//		else if (pre_thisRect.top <= pre_otherRect.bottom)
	//		{
	//			float delta = thisRect.top - otherRect.bottom;

	//			if (delta >= 0)
	//			{
	//				thisPos_->y -= delta ;
	//				//改めてプレイヤーの四つ角せってい
	//				thisRect.SetShape(*thisPos_, thisWidth_, thisHeight_);

	//				ret = true;
	//				colMapInfo_.isCeiling = true;

	//			}
	//		}
	//	}

	//}

	return ret;
}


