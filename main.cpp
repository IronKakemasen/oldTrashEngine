#include "./M/lowerLayer/engineCore/WinApp.h"
#include "./M/lowerLayer/M.h"
#include "M/lowerLayer/Scene/SceneController/SceneController.h"
#include "./M/lowerLayer/engineCore/Essential/leakChecker/leakChecker.h"
#include "../../utilities/Json/Json.h"
#include "./M/lowerLayer/engineCore/Audio/AudioManager.h"
#include "./M/lowerLayer/engineCore/Audio/AudioHandle.h"
#include "./M/lowerLayer/engineCore/Audio/AudioPlayer.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	////＜ポインタ破壊などを検知するためのもの＞
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_CHECK_ALWAYS_DF);

#ifdef _DEBUG
	//ResourceLeakChecker r;
#endif // DEBUG

	WinApp winApp((UINT)CommonV::kWindow_W, (UINT)CommonV::kWindow_H, L"2308_loool");

	//// AudioManagerの初期化
	//AudioManager::GetInstance().Initialize();
	//// AudioHandleの初期化
	//AudioHandle::Initialize();

	std::unique_ptr<SceneController> sceneController =
		std::make_unique<SceneController>(kModelView);

	Vertex vertexData[24];
	// 右面。描画インデックスは[0,1,2][2,1,3]で内側を向く
	vertexData[0].position = { 1.0f, 1.0f, 1.0f };
	vertexData[1].position = { 1.0f, 1.0f, -1.0f };
	vertexData[2].position = { 1.0f, -1.0f, 1.0f };
	vertexData[3].position = { 1.0f, -1.0f, -1.0f };

	// 左面。描画インデックスは[4,5,6][6,5,7]
	vertexData[4].position = { -1.0f, 1.0f, -1.0f };
	vertexData[5].position = { -1.0f, 1.0f, 1.0f };
	vertexData[6].position = { -1.0f, -1.0f, -1.0f };
	vertexData[7].position = { -1.0f, -1.0f, 1.0f };

	// 前面。描画インデックスは[8,9,10][10,9,11]
	vertexData[8].position = { -1.0f, 1.0f, 1.0f };
	vertexData[9].position = { 1.0f, 1.0f, 1.0f };
	vertexData[10].position = { -1.0f, -1.0f, 1.0f };
	vertexData[11].position = { 1.0f, -1.0f, 1.0f };

	// 後面。描画インデックスは[12,13,14][14,13,15]
	vertexData[12].position = { 1.0f, 1.0f, -1.0f };
	vertexData[13].position = { -1.0f, 1.0f, -1.0f };
	vertexData[14].position = { 1.0f, -1.0f, -1.0f };
	vertexData[15].position = { -1.0f, -1.0f, -1.0f };

	// 上面。描画インデックスは[16,17,18][18,17,19]
	vertexData[16].position = { -1.0f, 1.0f, 1.0f };
	vertexData[17].position = { -1.0f, 1.0f, -1.0f };
	vertexData[18].position = { 1.0f, 1.0f, 1.0f };
	vertexData[19].position = { 1.0f, 1.0f, -1.0f };

	// 下面。描画インデックスは[20,21,22][22,21,23]
	vertexData[20].position = { -1.0f, -1.0f, 1.0f };
	vertexData[21].position = { 1.0f, -1.0f, 1.0f };
	vertexData[22].position = { -1.0f, -1.0f, -1.0f };
	vertexData[23].position = { 1.0f, -1.0f, -1.0f };
	//=======この頂点座標をもとに頂点バッファは生成済み！=======

	MSG msg = {};
	while (WM_QUIT != msg.message)
	{
		//Windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//winApp.TestMeshShaderBegin();

			//winApp.TestMeshShaderDraw2(&CameraController::curUsingViewMat);
			////winApp.TestMeshShaderDraw(&CameraController::curUsingViewMat);

			//sceneController->Update();
			////sceneController->Draw();

			//winApp.TestMeshShaderEnd();

			//--------------------------------------------------------------------------------------------------

			winApp.OffScreenBegin();

			sceneController->Update();
			sceneController->Draw();
			

			winApp.OffScreenEnd();

			sceneController->DrawNonPostEffect();

			winApp.EndFrame();

		}
	}

	winApp.TermApp();

    return 0;
}

