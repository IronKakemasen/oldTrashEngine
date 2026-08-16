#pragma once
#include <dinput.h>

enum class KeyType
{
	ESCAPE = 0x01,
	Q = 0x10 ,
	W = 0x11 ,
	E = 0x12 ,
	R = 0x13 ,
	T = 0x14 ,
	Y = 0x15 ,
	U = 0x16 ,
	I = 0x17 ,
	O = 0x18 ,
	P = 0x19 ,
	A = 0x1E,
	S = 0x1F,
	D = 0x20,
	F = 0x21,
	G = 0x22,
	H = 0x23,
	J = 0x24,
	K = 0x25,
	L = 0x26,
	Z = 0x2C,
	X = 0x2D,
	C = 0x2E,
	V = 0x2F,
	B = 0x30,
	N = 0x31,
	M = 0x32,
	UP = 0xC8,
	DOWN = 0xD0,
	LEFT = 0xCB,
	RIGHT= 0xCD,
	SPACE = 0x39,
	SHIFT = 0x2A	
};	

class KeyboardKeys
{
	BYTE keys[256] = {};
	BYTE preKeys[256] = {};

public:

	inline BYTE* Getter_Key()
	{
		return keys;
	}

	bool IsKeyTrigger(KeyType key_);
	bool IsKeyPress(KeyType key_);
	void Copy();


};

