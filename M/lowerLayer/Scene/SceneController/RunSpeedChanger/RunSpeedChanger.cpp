#include "RunSpeedChanger.h"
#include "../../../../utilities/benriTemplateFunc/benriTempFunc.h"
#include <climits> 
#include "../M.h"

#ifdef USE_IMGUI
#include "imgui.h"
void RunSpeedChanger::ForDebug::GuiAdd_ChangeRunSpeed(int* runSpeed_)
{
	ImGui::Text("Stop     : "); ImGui::SameLine();
	ImGui::Checkbox("(SHIFT)", &stopButton);
	if (M::GetInstance()->IsKeyTriggered(KeyType::SHIFT))
	{
		stopButton = !stopButton;
	}

	if (stopButton)
	{
		*runSpeed_ = (int)RunSpeed::kStop;
		onlyOnce = true;
		ImGui::Text("STOPTYUU");
	}
	else
	{
		if (onlyOnce)
		{
			*runSpeed_ = runSpeedBuffer;
			onlyOnce = false;
			return;
		}
		runSpeedBuffer = *runSpeed_;
		ImGui::SliderInt("RunSpeed ", runSpeed_, (int)RunSpeed::kStop, (int)RunSpeed::kStandared);
	}

}
#endif // USE_


void RunSpeedChanger::AddDebug()
{
#ifdef _DEBUG
	forDebug.GuiAdd_ChangeRunSpeed(&runSpeed);
#endif // DEBUG

}

bool RunSpeedChanger::IsStop()
{
	if (runSpeed == (int)RunSpeed::kStop)
	{
		return true;
	}

	return false;
}

RunSpeedChanger::RunSpeedChanger()
{
	Reset();
}

void RunSpeedChanger::Reset()
{
	runSpeed = 10;		//min = 1,default = 10 (1 = stop, )
	framCnt = 0;
}

bool RunSpeedChanger::AdjustRunSpeed()
{
	//簡易的だけども
	if (runSpeed != 10)
	{
		Benri::Min(runSpeed, (int)RunSpeed::kStop);

		framCnt++;
		Benri::AdjustMax(framCnt, INT_MAX, 0);

		if (framCnt % runSpeed == 0)
		{
			return false;
		}
	}

	return true;
}


