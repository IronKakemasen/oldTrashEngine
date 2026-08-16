#include "../Quaternion/Quaternion.h"
#include "EasingFunctions.h"

Quaternion Easing::SlerpQuaternion(Quaternion q1_, Vector3 targetDir_, float t_)
{
	Quaternion result;

	Quaternion q2;
	q2.q = Quaternion::CreateQuaternion(targetDir_);

	float dot = q1_.GetDot(q2);

	if (dot < 0.0) 
	{
		q2.q = q2.q * -1.0;
		dot = -dot;
	}

	Benri::Max(dot, 1.0f);

	//非常に近い角度の場合の処理 (LERPへのフォールバック)
	const float dotThreshold = 0.9995f;
	if (dot > dotThreshold)
	{
		Quaternion result;
		result.q = q1_.q + (q2.q + (q1_.q* -1.0)) * t_;
		result.q = result.GetNormalized();
		return result;
	}

	//SLERPの計算
	float theta = std::acos(dot);
	float interpolatedTheta = theta * t_;
	float inv_sinT0 = 1.0f / sinf(theta);
	float sinT1 = sinf(interpolatedTheta);

	float s1 = sinf((1.0f - t_) * theta) * inv_sinT0;
	float s2 = sinT1 * inv_sinT0;

	result.q = (q1_.q * s1) + (q2.q * s2);
	
	return result;
}
