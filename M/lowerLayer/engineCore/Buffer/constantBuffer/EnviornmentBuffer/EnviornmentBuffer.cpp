#include "EnviornmentBuffer.h"
#include "../WinApp.h"


EnviornmentBuffer* EnviornmentBuffer::Get()
{
	static EnviornmentBuffer enviornmentBuffer;

	return &enviornmentBuffer;
}

EnviornmentBuffer::EnviornmentBuffer()
{
	enviornmentParam.CreateAndMapping(WinApp::deviceSetUp.Getter_Device());
	enviornmentParam.buffMap->groundColor = { };
	enviornmentParam.buffMap->skydomeColor = {};

}
