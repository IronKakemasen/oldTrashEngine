#include "SceneBehavior.h"
#include "../SceneController/SceneController.h"
#include "../../engineCore/WinApp.h"

void Scene::ChangeScene(SceneType next_)
{
	nextScene = next_;
}

SceneType Scene::SendSignal()
{
	return nextScene;
}

SceneBehavior::SceneBehavior() 
{
	gameObjManager.reset(new GameObjectManager);
	mainCamera.reset(new NormalCamera);
	cameraController.reset(new CameraController);
	cameraController->RegisterForContainer("mainCamera", mainCamera.get());
	cameraController->ChangeCamera("mainCamera");
	dirLight = M::GetInstance()->ImportDirLight();
	doReset = false;

}

void SceneBehavior::ActivateReset()
{
	doReset = true;
}


