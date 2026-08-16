#pragma once
#include "../../../utilities/benriTemplateFunc/benriTempFunc.h"
#include "./RunSpeedChanger/RunSpeedChanger.h"
#include "../FwdScenes.h"
#include "./AxisModel/AxisModel.h"
#include "SceneType.h"


class PointLight;

class SceneController
{
private:

	std::unique_ptr<ModelScene> modelViewScene;
	std::unique_ptr<AxisModel> axisModel;

	//現在稼働しているシーン
	SceneType runningScene;
	static inline SceneType nextScene;
	//すべてのシーン
	Scene* allScene[SceneType::kCount];
	//処理間隔を調整する
	RunSpeedChanger runSpeedChanger;
	std::vector<std::string> sceneNameContainer;

#ifdef _DEBUG
	struct ForDebug
	{
		//グリッド線を描画するかどうか
		bool doDrawGridLine = true;
		Vector3 lightBuffer;
	};

	ForDebug forDebug;
#endif // _DEBUG


public:
	void Update();
	void Draw();
	void DrawNonPostEffect();
	SceneController(SceneType firstScene_);
	static void ChangeScene(SceneType nextSceneType_);

private:
	//稼働するシーンを切り替える
	void ChangeScene();
	//シーンをインスタンス化する
	void InstantiateScenes();
	//シーンをセットする
	void SetScenesToArray();
	std::string GetName(SceneType type_);
	SceneType GetType(int index_);
	void Debug();
	void Set(SceneType type_, Scene* scene_);
	void Init(SceneType firstScene_);
	void Reset();

};

