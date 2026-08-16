#include "KeyboardKeys.h"

bool KeyboardKeys::IsKeyTrigger(KeyType key_)
{
	bool ret = false;

	if (keys[(int)key_] && !preKeys[(int)key_]) ret = true;

	return ret;
}

bool KeyboardKeys::IsKeyPress(KeyType key_)
{
	bool ret = false;

	if (keys[(int)key_]) ret = true;

	return ret;
}
void KeyboardKeys::Copy()
{
	memcpy(preKeys, keys, 256);
}

