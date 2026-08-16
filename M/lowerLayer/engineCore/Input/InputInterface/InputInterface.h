#pragma once
#define DIRECTINPUT_VERSION 0x0800//#define DIRECTINPUT_VERSION 0x0800

#include <Windows.h>
#include <dinput.h>
#include "../KeyboardKeys/KeyboardKeys.h"

class InputInterface
{
	IDirectInput8* directInput = nullptr;
	IDirectInputDevice8* keyboard = nullptr;
	//mouse
	IDirectInput8* directInputForMouse = nullptr;
	IDirectInputDevice8* mouse = nullptr;

public:
	KeyboardKeys keyboardKeys;

	void Initialize(HINSTANCE hInstance_, HWND hwnd_);
	void Finalize();
	void Update();

};
