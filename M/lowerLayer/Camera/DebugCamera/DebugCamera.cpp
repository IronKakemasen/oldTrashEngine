#include "DebugCamera.h"
#include "../../M.h"
#include "../../M/utilities/benriTemplateFunc/benriTempFunc.h"

DebugCamera::DebugCamera()
{
	mode = kParallel;
	nextMode = mode;
	sphericalCoordinates = { 7.5f,0.0f,-45.0f };
	centerPos = { 0.0f,0.0f,0.0f};

	action[Mode::kParallel] = std::bind(&DebugCamera::ParallelMove, this);
	action[Mode::kSpherical] = std::bind(&DebugCamera::SphericallyMove, this);
}

void DebugCamera::ChangeMode()
{
	auto* m = M::GetInstance();

	if (m->IsKeyTriggered(KeyType::V))
	{
		if (nextMode == Mode::kParallel)
		{
			nextMode = Mode::kSpherical;
			sphericalCoordinates = { 7.5f,0.0f,-45.0f };
			Vector3 buff = { sphericalCoordinates.x,GetRadian(sphericalCoordinates.y),
			GetRadian(sphericalCoordinates.z) };
			Vector3 tmp = ConvertToRectangularCoordinate(buff);
			centerPos = para.trans.pos - tmp;

			sphericalCoordinates = { 7.5f,0.0f,-45.0f };
			para.trans.interpolationCoe = 0.1f;
		}

		else
		{
			para.trans.lookDir = {0,0,1};
			nextMode = Mode::kParallel;
			para.trans.interpolationCoe = 0.05f;

		}
	}

	mode = nextMode;
}

void DebugCamera::Update()
{
	ChangeMode();
	action[mode]();
	SetViewProjectionMat();
}

DebugCamera::Mode* DebugCamera::GetNextMode()
{
	return &nextMode;
}

std::string DebugCamera::OutputModeInString()
{
	std::string modeString[Mode::kCount]
	{
		"Parallel",
		"Spherical",
	};

	return modeString[(int)mode];
}

void DebugCamera::SphericallyMove()
{
	auto* m = M::GetInstance();
	float const speed = 2.0f;

	if (m->IsKeyPressed(KeyType::LEFT)) sphericalCoordinates.y += speed;
	if (m->IsKeyPressed(KeyType::RIGHT)) sphericalCoordinates.y -= speed;

	if (m->IsKeyPressed(KeyType::UP))sphericalCoordinates.z += speed;
	if (m->IsKeyPressed(KeyType::DOWN))sphericalCoordinates.z -= speed;

	if (m->IsKeyPressed(KeyType::Z))sphericalCoordinates.x -= speed * 0.1f;
	if (m->IsKeyPressed(KeyType::X))sphericalCoordinates.x += speed * 0.1f;

	sphericalCoordinates.z = std::clamp(sphericalCoordinates.z, -179.5f, -0.5f);

	Vector3 buff = { sphericalCoordinates.x,GetRadian(sphericalCoordinates.y),
	GetRadian(sphericalCoordinates.z) };

	para.trans.pos = ConvertToRectangularCoordinate(buff) + centerPos;
	para.trans.lookDir = centerPos  - para.trans.pos;

}

void DebugCamera::ParallelMove()
{
	auto* m = M::GetInstance();

	float const speed = 5.0f;
	float const deltaTime = m->GetDeltaTime();

	if (m->IsKeyPressed(KeyType::LEFT))
	{
		para.trans.pos.x -= speed * deltaTime;
	}

	if (m->IsKeyPressed(KeyType::UP))
	{
		para.trans.pos.y += speed * deltaTime;
	}

	if (m->IsKeyPressed(KeyType::DOWN))
	{
		para.trans.pos.y -= speed * deltaTime;
	}

	if (m->IsKeyPressed(KeyType::RIGHT))
	{
		para.trans.pos.x += speed * deltaTime;
	}

	if (m->IsKeyPressed(KeyType::Z))
	{
		para.trans.pos.z += speed * deltaTime;
	}

	if (m->IsKeyPressed(KeyType::X))
	{
		para.trans.pos.z -= speed * deltaTime;
	}
}
