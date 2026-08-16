#pragma once
#include "../CameraBehavior.h"
#include <string>

struct DebugCamera :Camera
{

private:

	enum Mode
	{
		kParallel,
		kSpherical,
		kCount
	};

	Vector3 centerPos;
	Vector3 sphericalCoordinates;
	Mode mode;
	Mode nextMode;
	std::unordered_map<Mode , std::function<void()>> action;
	
	void ChangeMode();
	void ParallelMove();
	void SphericallyMove();

public:
	Mode* GetNextMode();
	virtual void Update()override;
	std::string OutputModeInString();
	DebugCamera();

};

