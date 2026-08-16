#include "InputInterface.h"
#include <assert.h>
#pragma comment(lib,"dinput8.lib")

void InputInterface::Update()
{
	keyboardKeys.Copy();

	//キーボード情報の取得
	keyboard->Acquire();
	keyboard->GetDeviceState(sizeof(*keyboardKeys.Getter_Key()) *
		256, keyboardKeys.Getter_Key());
}


void InputInterface::Initialize(HINSTANCE hInstance_, HWND hwnd_)
{
	//DirectInputの初期化
	HRESULT hr = DirectInput8Create(hInstance_, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(hr));
	//キーボードデバイスの生成
	hr = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(hr));
	//入力データ形式のセット
	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));
	//排他制御レベルのセット
	hr = keyboard->SetCooperativeLevel(
		hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));
}

void InputInterface::Finalize()
{
	keyboard->Unacquire();
	//mouse->Unacquire();
	directInput->Release();
	//directInputForMouse->Release();
	keyboard->Release();
	//mouse->Release();
}