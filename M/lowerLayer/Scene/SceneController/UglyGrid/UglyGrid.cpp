#include "UglyGrid.h"
#include "../../M.h"

void UglyGrid::Draw(Matrix4* vpMat_)
{
#ifdef _DEBUG
	static float const len = 1.0f;
	static float const kNumLines = 120;
	static float const half = kNumLines * 0.5f;

	Vector3 xLineStart = { -half,0,-half };
	Vector3 xLineEnd = { half,0,-half };
	Vector3 zLineStart = { -half,0,half };
	Vector3 zLineEnd = { -half,0,-half };

	Vector4 xColor = { 0,75,0,200 };
	Vector4 zColor = { 75,0,0,200 };
	Vector4 zero = { 0,0,25,255 };
	Vector4 cur_xColor = zero;
	Vector4 cur_zColor = zero;

	static float inv = 1.0f / kNumLines;

	for (int i = 0; i < kNumLines; ++i)
	{

		if (i == (int)half)
		{
			cur_xColor = xColor;
			cur_zColor = zColor;
		}

		else if ((i % 10) == 0)
		{
			int digit = i / 10;
			float const standardVal = 10.0f;
			Vector4 color = { standardVal * digit ,standardVal * digit ,standardVal * digit ,255.0f };
			cur_xColor = color;
			cur_zColor = color;
		}

		else
		{
			cur_xColor = zero;
			cur_xColor.x = 125 * (xLineStart.z * inv);
			cur_zColor = zero;
			cur_zColor.y = 125 * (zLineStart.x * inv);

		}

		M::GetInstance()->DrawLine(xLineStart, xLineEnd,cur_xColor, vpMat_);
		M::GetInstance()->DrawLine(zLineStart, zLineEnd,cur_zColor, vpMat_);

		xLineStart.z++;
		xLineEnd.z++;

		zLineStart.x++;
		zLineEnd.x++;

	}
#endif // DEBUG

}