#pragma once
#include "../SceneBehavior/SceneBehavior.h"
#include "../../Particle/2D/TestParticle/TestParticle.h"
#include "../../GameObject/Sample/SamplePlayer/SamplePlayer.h"
#include "../../GameObject/Sample/SampleEnemy/SampleEnemy.h"
#include "../../GameObject/Sample/SampleObj/SampleObj.h"


class SampleScene final : public Scene
{
public:
	SampleScene();

	std::unique_ptr<MTriangle> triangle;
	std::unique_ptr<MQuad> quad;
	std::unique_ptr<SamplePlayer> sPlayer;
	std::unique_ptr<SampleObj> sObj;

	std::vector<std::unique_ptr<SampleEnemy>> sEnemies;

	virtual void Update() override;
	virtual void Draw() override;
	virtual void Reset() override;
	virtual void Debug() override;
	virtual void Instantiate() override;
	virtual void Init() override;

};

