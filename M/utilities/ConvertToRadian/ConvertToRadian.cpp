#include "ConvertToRadian.h"

float GetRadian(float rotate_)
{
	static float const inv180Pi = 1.0f / 180.0f * 3.1415927f;
	return rotate_ * inv180Pi;
}
