#pragma once
#include <memory>
#include "../../Camera/NormalCamera/NormalCamera.h"
#include "../../Camera/CameraController/CameraController.h"
#include "../../engineCore/Light/DirectionalLight/DirectionalLight.h"
#include "../../engineCore/Light/PointLight/PointLight.h"
#include "../../GameObjectManager/GameObjectManager.h"
#include "../../MobilePrimitive/MobileQuad/MQuad/MQuad.h"
#include "../../MobilePrimitive/Sprite/Sprite.h"
#include "../SceneController/SceneType.h"
#include "../../engineCore/Buffer/constantBuffer/EnviornmentBuffer/EnviornmentBuffer.h"


#ifdef USE_IMGUI
#include "../../../../external/imgui/imgui.h"
#endif // USE_IMGUI

class SceneBehavior
{
public:
	SceneBehavior();

	std::unique_ptr < CameraController > cameraController;
	std::unique_ptr< NormalCamera > mainCamera;
	std::unique_ptr< GameObjectManager > gameObjManager;
	DirectionalLight* dirLight = nullptr;

	static inline bool doReset = false;

	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Reset() = 0;
	virtual void Debug() = 0;
	virtual void Instantiate() = 0;
	virtual void Init() = 0;
	static void ActivateReset();

};

class Scene : public SceneBehavior
{
public:
	Scene() {};
	SceneType nextScene;

	void ChangeScene(SceneType next_);
	SceneType SendSignal();
	virtual void Update()override {};
	virtual void Draw() override {};
	virtual void Reset() override {};
	virtual void Debug() override {};
	virtual void Instantiate() override {};
	virtual void Init() override {};
};



