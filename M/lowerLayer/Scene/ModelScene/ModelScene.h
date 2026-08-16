#pragma once
#include "../SceneBehavior/SceneBehavior.h"
#include "./AAA/AAA.h"

class ModelScene final : public Scene
{

public:

	static inline std::unique_ptr<AAA> aaa;

	ModelScene();

	virtual void Update() override;
	virtual void Draw() override;
	virtual void Reset() override;
	virtual void Debug() override;
	virtual void Instantiate() override;
	virtual void Init() override;

};

