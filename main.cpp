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

