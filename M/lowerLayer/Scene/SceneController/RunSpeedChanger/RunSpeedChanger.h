#pragma once

class RunSpeedChanger
{
	enum RunSpeed
	{
		kStop = 1,
		kHalf = 5,
		kStandared = 10
	};

	int runSpeed;		//min = 1,default = 10 (1 = stop, )
	int framCnt;

#ifdef USE_IMGUI
	struct ForDebug
	{
	private:
		bool stopButton = false;
		int runSpeedBuffer = 10;
		bool onlyOnce = true;
	public:
		void GuiAdd_ChangeRunSpeed(int* runSpeed_);
	};

	ForDebug forDebug;

#endif // _DEBUG


public:
	RunSpeedChanger();
	bool AdjustRunSpeed();
	void Reset();
	void AddDebug();
	bool IsStop();
};

