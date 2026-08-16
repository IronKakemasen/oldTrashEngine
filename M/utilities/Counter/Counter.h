#pragma once

struct Counter
{
private  :
	float inv_Time;

public:
	float count = 0.0f;
	float speed = 1.0f;

	Counter(float performTime_);
	Counter(){}
	void Add(float deltaTime_ = 0.016666666f);
	void Substract(float deltaTime_);
	void Initialize(float sumFrame_);
	bool IsEnd();
	static float GetNormalizedCount(float st_, float end_, float cur_Cnt_);

};

