#include "Rect.h"

void Rect::SetShape(float width_, float height_, Vector3 center_)
{
	left = -width_ * 0.5f + center_.x;
	right = width_ * 0.5f + center_.x;
	bottom = -height_ * 0.5f + center_.z;
	top = height_ * 0.5f + center_.z;

}

Rect Rect::ConvertToWorld(Vector3 center_)
{
	Rect ret = *this;

	ret.left += center_.x;
	ret.right += center_.x;
	ret.bottom += center_.z;
	ret.top += center_.z;

	return ret;
}
