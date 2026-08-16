#pragma once
#include <functional>
#include "../benriTemplateFunc/benriTempFunc.h"

struct Quaternion;

//sin(θ(1−t))sin(θ)
namespace Easing
{
	template<typename T>
	static inline T Lerp(const T& st_, const T& end_, float t_)
	{
		return end_ * t_ + st_ * (1.0f - t_);
	}

	static inline Vector3 SLerp(Vector3 thisDir_, Vector3 otherDir_, float t_)
	{
		Vector3 ret;
		thisDir_ = thisDir_.GetNormalized();
		otherDir_ = otherDir_.GetNormalized();
		ret = thisDir_;
		//* 57.2958f
		float arcCos = std::acos(thisDir_.GetDot(otherDir_));

		if (sinf(arcCos) >= 1.0e-5)  
		{
			float invSinT = 1.0f / sinf(arcCos);

			float len = std::sinf(arcCos * (1.0f - t_)) * invSinT;
			float len2 = std::sinf(arcCos * t_) * invSinT;
			ret = thisDir_ * len + otherDir_ * len2;
		}

		return ret;
	}

	Quaternion SlerpQuaternion(Quaternion q1_, Vector3 targetDir_, float t_);

	template<typename T>
	static inline T EaseInBack(const T& st_, const T& end_, float t_)
	{
		float const c1 = 1.70158f;
		float const c3 = c1 + 1.0f;

		float convertedT = c3 * t_ * t_ * t_ - c1 * t_ * t_;

		return Lerp(st_, end_, convertedT);
	}

	template<typename T>
	static inline T EaseOutBack(const T& st_, const T& end_, float t_)
	{
		float const c1 = 1.70158f;
		float const c3 = c1 + 1.0f;

		float convertedT = 1.0f + c3 * powf(t_ - 1.0f, 3.0f) + c1 * powf(t_ - 1.0f, 2.0f);
		Benri::Max(convertedT, 1.0f);

		return Lerp(st_, end_, convertedT);
	}

	template<typename T>
	static inline T EaseOutCubic(const T& st_, const T& end_, float t_)
	{
		float convertedT = 1.0f - (float)pow(1.0f - t_, 3.0f);

		return Lerp(st_, end_, convertedT);
	}

	template<typename T>
	static inline T EaseInExpo(const T& st_, const T& end_, float t_)
	{
		float convertedT = (float)powf(2.0f, 10.0f * t_ - 10.0f);

		return Lerp(st_, end_, convertedT);
	}

	template<typename T>
	static inline T EaseOutBounce(const T& st_, const T& end_, float t_)
	{
		float const n1 = 7.5625f;
		float const d1 = 2.75f;

		float convertedT;

		if (t_ < 1.0f / d1)
		{
			convertedT = n1 * t_ * t_;
		}

		else if (t_ < 2.0f / d1)
		{
			convertedT = n1 * (t_ -= 1.5f / d1 )* t_ +0.75f;
		}

		else if (t_ < 2.5f / d1)
		{
			convertedT = n1 * (t_ -= 2.25f / d1) * t_ + 0.9375f;
		}

		else
		{
			convertedT = n1 * (t_ -= 2.625f / d1) * t_ + 0.984375f;
		}

		return Lerp(st_, end_, convertedT);
	}

	template<typename T>
	static inline T EaseOutElastic(const T& st_, const T& end_, float t_)
	{
		float convertedT;
		static float const c4 = (2 * 3.14159265359f) / 3.0f;
		convertedT	= powf(2.0f, -10.0f * t_)* sinf((t_ * 10.0f - 0.75f) * c4) + 1.0f;

		return Lerp(st_, end_, convertedT);
	}

	template<typename T>
	static inline T EaseOutExpo(const T& st_, const T& end_, float t_)
	{
		float convertedT;
		convertedT = 1.0f - powf(2.0f, -10.0f * t_);

		return Lerp(st_, end_, convertedT);

	}

	template<typename T>
	static inline T EaseOutQuint(const T& st_, const T& end_, float t_)
	{
		float convertedT;
		convertedT = 1.0f - powf(1.0f - t_, 5.0f);

		return Lerp(st_, end_, convertedT);

	}

	template<typename T>
	static inline T EaseOutSine(const T& st_, const T& end_, float t_)
	{
		float convertedT;
		convertedT = sinf((t_ * 3.14f) * 0.5f);

		return Lerp(st_, end_, convertedT);

	}

	template<typename T>
	static inline T EaseInCirc(const T& st_, const T& end_, float t_)
	{
		float convertedT;
		convertedT = 1.0f - sqrtf(1.0f - (t_ * t_));

		return Lerp(st_, end_, convertedT);
	}

	template<typename T>
	static inline T EaseOutCirc(const T& st_, const T& end_, float t_)
	{
		float convertedT;
		convertedT = sqrtf(1.0f - (t_ - 1.0f)* (t_ - 1.0f));

		return Lerp(st_, end_, convertedT);
	}

	template<typename T>
	static inline T EaseInOutQuad(const T& st_, const T& end_, float t_)
	{
		float convertedT;

		if (t_ < 0.5f)
		{
			convertedT = 2.0f * t_ * t_;
		}
		else
		{
			convertedT = 1.0f - (-2.0f * t_ + 2.0f) * (-2.0f * t_ + 2.0f) * 0.5f;
		}
		return Lerp(st_, end_, convertedT);
	}



}
	namespace ConvertOnly
	{
		template<typename T>
		static inline T InBack(const T& st_, const T& end_, float t_)
		{
			float const c1 = 1.70158f;
			float const c3 = c1 + 1.0f;

			return  c3 * t_ * t_ * t_ - c1 * t_ * t_;
		}

	}


