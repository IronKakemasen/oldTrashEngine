#include "Counter.h"

Counter::Counter(float performTime_)
{
	Initialize(performTime_);
}

//設定した加算カウント値をカウンターにいれる
void Counter::Add(float deltaTime_)
{
	if (count < 1.0f)
	{
		count += inv_Time * deltaTime_* speed;
		if (count > 1.0f)
		{
			count = 1.0f;
		}
	}
}

void Counter::Substract(float deltaTime_)
{
	if (count > 0.0f)
	{
		count -= inv_Time * deltaTime_;
		if (count < 0.0f)
		{
			count = 0.0f;
		}

	}
}

//加算カウント値を設定する
void Counter::Initialize(float sumFrame_)
{
	inv_Time = 1.0f / sumFrame_;
	count = 0.0f;

}

//終了を返す関数
bool Counter::IsEnd()
{
	bool ret = false;

	if (count >= 1.0f)
	{
		count = 0.0f;
		ret = true;
	}

	return ret;
}

float Counter::GetNormalizedCount(float st_, float end_, float cur_Cnt_)
{
	return (cur_Cnt_ - st_) * (1.0f / (end_ - st_));
}
