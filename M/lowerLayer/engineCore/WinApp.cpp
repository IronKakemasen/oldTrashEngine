#include "WinApp.h"
#include "./Essential/BarrierControl/BarrierControl.h"
#include <assert.h>
#include "../M.h"
#include <dxgidebug.h>
#include "./Buffer/constantBuffer/Time/TimeConstBuffer.h"

void WinApp::TestMeshShaderDraw2(Matrix4* vpMat_)
{
	auto* cList = commandControl.Getter_commandList();
	auto* resMesh = testMeshShader.blockModel->model->Getter_ModelDataOfResMeshes(0);

	Matrix4 world = testMeshShader.trans.GetWorldMatrix();
	Matrix4 wvp = world.Multiply(*vpMat_);

	world = testMeshShader.trans.GetWorldMatrix();
	wvp = world.Multiply(*vpMat_);
	testMeshShader.transformMatrixBuffer.matrix.buffMap->wvp = wvp;
	testMeshShader.transformMatrixBuffer.matrix.buffMap->world = world;
	
	cList->SetGraphicsRootSignature(testMeshShader.rootSignature2.Get());
	cList->SetPipelineState(testMeshShader.pipelineStateObject2.Get());

	cList->SetGraphicsRootDescriptorTable(0,
		exclusiveDraw.shaderBufferData->gpuHandleContainer[resMesh->vertexData.gpuHandleIndex]);

	cList->SetGraphicsRootDescriptorTable(1,
		exclusiveDraw.shaderBufferData->gpuHandleContainer[resMesh->uniqueVertexIndexData.gpuHandleIndex]);

	cList->SetGraphicsRootDescriptorTable(2,
		exclusiveDraw.shaderBufferData->gpuHandleContainer[resMesh->meshletData.gpuHandleIndex]);

	cList->SetGraphicsRootDescriptorTable(3,
		exclusiveDraw.shaderBufferData->gpuHandleContainer[resMesh->primitiveIndicesData.gpuHandleIndex]);

	cList->SetGraphicsRootDescriptorTable(4,
		exclusiveDraw.shaderBufferData->gpuHandleContainer[testMeshShader.blockModel->model->GetAppearance(0)->use_texHandles[0]]);

	cList->SetGraphicsRootConstantBufferView(5, testMeshShader.transformMatrixBuffer.matrix.GetVirtualGPUAddress());

	cList->DispatchMesh((UINT)resMesh->meshlets.size(), 1, 1);

}


void WinApp::TestMeshShaderDraw(Matrix4* vpMat_)
{
	auto* cList = commandControl.Getter_commandList();

	Matrix4 world = testMeshShader.trans.GetWorldMatrix();
	Matrix4 wvp = world.Multiply(*vpMat_);
	
	world = testMeshShader.trans.GetWorldMatrix();
	wvp = world.Multiply(*vpMat_);

	testMeshShader.transformMatrixBuffer.matrix.buffMap->wvp = wvp;
	//testMeshShader.transformMatrixBuffer.matrix.buffMap->world = world;

	cList->SetGraphicsRootSignature(testMeshShader.rootSignature.Get());
	cList->SetPipelineState(testMeshShader.pipelineStateObject.Get());

	cList->SetGraphicsRootDescriptorTable(0,
		exclusiveDraw.shaderBufferData->gpuHandleContainer[testMeshShader.vertexStructuredBuffer.vertex.gpuHandleIndex]);

	cList->SetGraphicsRootDescriptorTable(1,
		exclusiveDraw.shaderBufferData->gpuHandleContainer[testMeshShader.indexStructuredBuffer.indices.gpuHandleIndex]);


	cList->SetGraphicsRootConstantBufferView(2, testMeshShader.transformMatrixBuffer.matrix.GetVirtualGPUAddress());

	cList->DispatchMesh(1, 1, 1);
}

void WinApp::TestMeshShaderBegin()
{
	auto* cList = commandControl.Getter_commandList();

	//ライトの更新処理
	lightManager.Update();

	//入力インターフェースの更新
	inputInterface.Update();

	QueryPerformanceFrequency(&fpsController.mTimeFreq);
	QueryPerformanceCounter(&fpsController.mTimeStart);

	auto* m = M::GetInstance();
	m->getPadState.Update();

	m->passedTime += 0.0166666f;
	TimeConstBuffer::Get()->buffer.buffMap->passedTime = m->passedTime;
	if (m->passedTime >= INT_MAX)m->passedTime = 0.0f;

	commandControl.PrepareForNextCommandList();

	//Imguiにここからフレームが始まる旨を告げる
#ifdef USE_IMGUI
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif

	D3D12_RESOURCE_BARRIER barrier = BarrierControl::Create(
		swapChainControl.Getter_ColorBuffer(swapChainControl.frameIndex)->Getter_Resource(),
		D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		D3D12_RESOURCE_BARRIER_FLAG_NONE,
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES );

	BarrierControl::Pitch(cList, &barrier);

	cList->OMSetRenderTargets(1, swapChainControl.Getter_ColorBuffer(swapChainControl.frameIndex)->Getter_Handle(),
		false, swapChainControl.Getter_DepthBuffer()->Getter_Handle());


	cList->ClearDepthStencilView(*swapChainControl.Getter_DepthBuffer()->Getter_Handle(),
		D3D12_CLEAR_FLAG_DEPTH, 0.0f, 0, 0, nullptr);

	cList->ClearRenderTargetView(
		*swapChainControl.Getter_ColorBuffer(swapChainControl.frameIndex)->Getter_Handle(), windowColor, 0, nullptr);

	ID3D12DescriptorHeap* descriptorHeaps[] = { srvDescHeap.Getter_Descriptorheap() };
	cList->SetDescriptorHeaps(1, descriptorHeaps);

	//DXの行列の設定
	cList->RSSetViewports(1,
		swapChainControl.Getter_ColorBuffer(swapChainControl.frameIndex)->Getter_ViewportMatrix());

	cList->RSSetScissorRects(1,
		swapChainControl.Getter_ColorBuffer(swapChainControl.frameIndex)->ScissorRect());



}
void WinApp::BeginFrame()
{
	//バリア
	D3D12_RESOURCE_BARRIER barrier = BarrierControl::Create(
		swapChainControl.Getter_ColorBuffer(swapChainControl.frameIndex)->Getter_Resource(),
		D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		D3D12_RESOURCE_BARRIER_FLAG_NONE,
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);


	//TransitionBarrierを張る
	BarrierControl::Pitch(commandControl.Getter_commandList(), &barrier);

	// レンダーゲットの設定.
	commandControl.Getter_commandList()->OMSetRenderTargets(1,
		swapChainControl.Getter_ColorBuffer(swapChainControl.frameIndex)->Getter_Handle(), false,
		swapChainControl.Getter_DepthBuffer()->Getter_Handle());

	//指定した深度で画面クリアする
	//commandControl.Getter_commandList()->ClearDepthStencilView(*swapChainControl.Getter_DepthBuffer()->Getter_Handle(),
	//	D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	commandControl.Getter_commandList()->ClearDepthStencilView(*swapChainControl.Getter_DepthBuffer()->Getter_Handle(),
		D3D12_CLEAR_FLAG_DEPTH, 0.0f, 0, 0, nullptr);

	//指定した色で画面全体をクリアする
	commandControl.Getter_commandList()->ClearRenderTargetView(
		*swapChainControl.Getter_ColorBuffer(swapChainControl.frameIndex)->Getter_Handle(), windowColor, 0, nullptr);

	////描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvDescHeap.Getter_Descriptorheap() };
	commandControl.Getter_commandList()->SetDescriptorHeaps(1, descriptorHeaps);

	//DXの行列の設定
	commandControl.Getter_commandList()->RSSetViewports(1,
		swapChainControl.Getter_ColorBuffer(swapChainControl.frameIndex)->Getter_ViewportMatrix());

	commandControl.Getter_commandList()->RSSetScissorRects(1,
		swapChainControl.Getter_ColorBuffer(swapChainControl.frameIndex)->ScissorRect());

}

void WinApp::EndFrame()
{
	//drawIndexをリセットする
	exclusiveDraw.ResetDrawIndexes();

	//[ 画面に書く処理が終わり、画面に映すので状態を遷移 ]

#ifdef USE_IMGUI
	ImGui::Render();
	//実際のcommandListのImguiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandControl.Getter_commandList());
#endif

	//バリア
	//RenderTarget->Prsent
	D3D12_RESOURCE_BARRIER barrier = BarrierControl::Create(
		swapChainControl.Getter_ColorBuffer(swapChainControl.frameIndex)->Getter_Resource(),
		D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		D3D12_RESOURCE_BARRIER_FLAG_NONE,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

	//TransitionBarrierを張る
	BarrierControl::Pitch(commandControl.Getter_commandList(), &barrier);

	//コマンドリストの内容を確定させる
	HRESULT hr = commandControl.Getter_commandList()->Close();
	assert(SUCCEEDED(hr));

	//GPUにコマンドリストの実行を行わさせる
	ID3D12CommandList* commandLists[] = { commandControl.Getter_commandList() };
	commandControl.Getter_CommandQueue()->ExecuteCommandLists(1, commandLists);
	//GPUとOSに画面の交換を行うように通知する
	swapChainControl.Getter_SwapChain()->Present(1, 0);

	//イベントを待つ
	fenceControl.WaitFenceEvent(commandControl.Getter_CommandQueue(), swapChainControl.Getter_SwapChain());

	fpsController.TimeAdjust();

}


void WinApp::TestMeshShaderEnd()
{
	//drawIndexをリセットする
	exclusiveDraw.ResetDrawIndexes();

	//[ 画面に書く処理が終わり、画面に映すので状態を遷移 ]

#ifdef USE_IMGUI
	ImGui::Render();
	//実際のcommandListのImguiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandControl.Getter_commandList());
#endif

	//バリア
	//RenderTarget->Prsent
	D3D12_RESOURCE_BARRIER barrier = BarrierControl::Create(
		swapChainControl.Getter_ColorBuffer(swapChainControl.frameIndex)->Getter_Resource(),
		D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		D3D12_RESOURCE_BARRIER_FLAG_NONE,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

	//TransitionBarrierを張る
	BarrierControl::Pitch(commandControl.Getter_commandList(), &barrier);

	//コマンドリストの内容を確定させる
	HRESULT hr = commandControl.Getter_commandList()->Close();
	assert(SUCCEEDED(hr));

	ID3D12CommandList* commandLists[] = { commandControl.Getter_commandList() };
	commandControl.Getter_CommandQueue()->ExecuteCommandLists(1, commandLists);
	swapChainControl.Getter_SwapChain()->Present(1, 0);

	//イベントを待つ
	fenceControl.WaitFenceEvent(commandControl.Getter_CommandQueue(), swapChainControl.Getter_SwapChain());

	fpsController.TimeAdjust();

}



void WinApp::OffScreenBegin()
{
	//Imguiにここからフレームが始まる旨を告げる
#ifdef USE_IMGUI
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif

	//ライトの更新処理
	lightManager.Update();

	//入力インターフェースの更新
	inputInterface.Update();

	QueryPerformanceFrequency(&fpsController.mTimeFreq);
	QueryPerformanceCounter(&fpsController.mTimeStart);

	auto* m = M::GetInstance();
	m->getPadState.Update();

	m->passedTime += 0.0166666f;
	TimeConstBuffer::Get()->buffer.buffMap->passedTime = m->passedTime;
	if (m->passedTime >= INT_MAX)m->passedTime = 0.0f;

	auto* cList = commandControl.Getter_commandList();
	auto* originalScreen = offScreenManager.GetOriginalScreen();
	auto* depthHandle = originalScreen->buffers[0]->depthBuffer.Getter_Handle();
	auto* rtvHandle = originalScreen->GetBuffer(0)->GetRtvHandle();
	auto* dof_rtvHandle = offScreenManager.dOF_Screen->GetBuffer(0)->GetRtvHandle();
	
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2] =
	{
		*rtvHandle,*dof_rtvHandle
	};

	// コマンドの記録を開始.
	commandControl.PrepareForNextCommandList();

	// ここで AllPipelineSet に現在のコマンドリストを渡す（commandControl.Getter_commandList() が有効であること）
	allPipelineSet.Setter_CommandlistForAll(commandControl.Getter_commandList());

	{
		char buf[256];
		auto curCL = commandControl.Getter_commandList();
		sprintf_s(buf, "\n\nCurrent commandList ptr (WinApp) = %p\n\n", curCL);
		Log(WinApp::log, std::string(buf));
	}

	D3D12_RESOURCE_BARRIER barrier = BarrierControl::Create(
		originalScreen->GetBuffer(0)->GetShaderBuffer()->Getter_Resource(),
		D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		D3D12_RESOURCE_BARRIER_FLAG_NONE,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

	D3D12_RESOURCE_BARRIER dof_barrier = BarrierControl::Create(
		offScreenManager.dOF_Screen->GetBuffer(0)->GetShaderBuffer()->Getter_Resource(),
		D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		D3D12_RESOURCE_BARRIER_FLAG_NONE,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

	BarrierControl::Pitch(commandControl.Getter_commandList(), &barrier);
	BarrierControl::Pitch(commandControl.Getter_commandList(), &dof_barrier);

	cList->OMSetRenderTargets(2, rtvHandles, false, depthHandle);

	//指定した深度で画面クリアする
	//cList->ClearDepthStencilView(*depthHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	cList->ClearDepthStencilView(*depthHandle, D3D12_CLEAR_FLAG_DEPTH, 0.0f, 0, 0, nullptr);

	//指定した色で画面全体をクリアする
	float clearColor[] = { 0.0f,0.0f,0.0f,0.0f };

	float palleteColor[4] = { 0.0f,0.0f,0.0f,0.0f };
	//float palleteColor2[4] = { 1.0f,1.0f,0.0f,0.0f };

	//指定した色で画面全体をクリアする
	commandControl.Getter_commandList()->ClearRenderTargetView(
		*rtvHandle, palleteColor, 0, nullptr);

	commandControl.Getter_commandList()->ClearRenderTargetView(
		*dof_rtvHandle, palleteColor, 0, nullptr);

	////描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvDescHeap.Getter_Descriptorheap() };
	commandControl.Getter_commandList()->SetDescriptorHeaps(1, descriptorHeaps);

	D3D12_VIEWPORT viewports[2] =
	{
		*originalScreen->buffers[0]->GetColorBuffer()->Getter_ViewportMatrix(),
		*offScreenManager.dOF_Screen->buffers[0]->GetColorBuffer()->Getter_ViewportMatrix()
	};

	D3D12_RECT rects[2] =
	{
		*originalScreen->buffers[0]->GetColorBuffer()->ScissorRect(),
		*offScreenManager.dOF_Screen->buffers[0]->GetColorBuffer()->ScissorRect()
	};

	//行列
	commandControl.Getter_commandList()->RSSetViewports(2,viewports);
	commandControl.Getter_commandList()->RSSetScissorRects(2,rects);


	//ID3D12Device::CreatePipelineState()
	//deviceSetUp.Getter_Device().


}

void WinApp::OffScreenEnd()
{
	auto* originalScreen = offScreenManager.GetOriginalScreen();

	D3D12_RESOURCE_BARRIER barrier = BarrierControl::Create(
		originalScreen->GetBuffer(0)->GetShaderBuffer()->Getter_Resource(),
		D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		D3D12_RESOURCE_BARRIER_FLAG_NONE,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

	D3D12_RESOURCE_BARRIER dof_barrier = BarrierControl::Create(
		offScreenManager.dOF_Screen->GetBuffer(0)->GetShaderBuffer()->Getter_Resource(),
		D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		D3D12_RESOURCE_BARRIER_FLAG_NONE,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

	BarrierControl::Pitch(commandControl.Getter_commandList(), &barrier);
	BarrierControl::Pitch(commandControl.Getter_commandList(), &dof_barrier);

	
	//postEffect
	offScreenManager.Update();

	BeginFrame();

	exclusiveDraw.DrawOnPalette(offScreenManager.GetPalette());
	//drawIndexをリセットする
	exclusiveDraw.ResetDrawIndexes();
}


bool WinApp::InitD3D()
{
	//COMの初期化
	CoInitializeEx(0, COINITBASE_MULTITHREADED);

	srand(unsigned int(time(NULL)));

	log = DebugLogInitialize();

	//デバイス
	deviceSetUp.Initialize();

	//[ デバッグヘルパー ]	
	//誰も捕捉しなかった場合に(Unhandled)、補足する関数を登録
	SetUnhandledExceptionFilter(ExportDump);

	//コンパイラーの初期化
	dxCompile.Initialize();

	//シェーダーテーブルの設定
	vpShaders.Init(&dxCompile);

	//コマンドキュー
	commandControl.MakeCommandQueue(deviceSetUp.Getter_Device());

	//rtv、srv、dsvそれぞれのdescriptorHeapを作成する
	rtvDescHeap.Init(deviceSetUp.Getter_Device(), 16);
	srvDescHeap.Init(deviceSetUp.Getter_Device(), 256);
	dsvDescriptorHeap.Init(deviceSetUp.Getter_Device(), 12);

	//swapChainの設定
	swapChainControl.Initialize(
		&m_hWnd, commandControl.Getter_CommandQueue(), deviceSetUp.Getter_DxgiFactory(),
		deviceSetUp.Getter_Device(), &rtvDescHeap,&dsvDescriptorHeap);

	//コマンドアローケータの生成
	commandControl.MakeCommandAllocator(deviceSetUp.Getter_Device());

	//コマンドリストの生成
	commandControl.MakeCommandList(deviceSetUp.Getter_Device());

	//FenceControl
	fenceControl.Initialize(deviceSetUp.Getter_Device(), deviceSetUp.Getter_DxgiFactory());

	//pipelineSetの初期化
	allPipelineSet.Initialize(deviceSetUp.Getter_Device(), &vpShaders, commandControl.Getter_commandList());
	
	//srvCreatorの初期化
	srvCreator.Init(&srvDescHeap,deviceSetUp.Getter_Device(),&commandControl , &shaderBufferData);

	//textureDataManager,textureDataCreatorの初期化
	textureDataManager.Init(srvCreator.Getter_TextureSrCreator(), 
		srvCreator.Getter_PostEffectSrCreator(),&rtvDescHeap,deviceSetUp.Getter_Device());

	//meshCreatorの初期化
	meshCreator.Init(&allPipelineSet, deviceSetUp.Getter_Device(),
		&textureDataManager,&commandControl,&fenceControl,swapChainControl.Getter_SwapChain());

	//cameraParameterSetterの初期化
	cameraParameterSetter.Init(deviceSetUp.Getter_Device());

	//exclusiveDrawの初期化
	exclusiveDraw.Init(&allPipelineSet, &allMesh, 
		&shaderBufferData, cameraParameterSetter.Getter_Buffer());


#ifdef USE_IMGUI
	//ImGuiの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsClassic();
	ImGui_ImplWin32_Init(m_hWnd);
	ImGui_ImplDX12_Init(
		deviceSetUp.Getter_Device(),
		swapChainControl.Ref_SwapChainDesc().BufferCount,
		swapChainControl.Ref_RenderTargetDesc().Format,
		srvDescHeap.Getter_Descriptorheap(),
		srvDescHeap.Getter_Descriptorheap()->GetCPUDescriptorHandleForHeapStart(),		
		srvDescHeap.Getter_Descriptorheap()->GetGPUDescriptorHandleForHeapStart());
	
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.5f, 0.5f, 0.1f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
	ShaderBuffer::cur_index++;

#endif

	//メッシュの初期化、生成
	allMesh.Init(deviceSetUp.Getter_Device(), srvCreator.Getter_ParticleMeshSrvCreator(),&allPipelineSet);
	
	//lightManager,lightCreatorの初期化
	lightManager.Init(&exclusiveDraw, deviceSetUp.Getter_Device(), srvCreator
		.Getter_StBufferCretaor());

	//テクスチャ読み込み含む（コマンド積む）
	M::GetInstance()->Init(&textureDataManager, &exclusiveDraw,
		vpShaders.Getter_VPShaderTable(), &allPipelineSet, &meshCreator, &lightManager,
		&cameraParameterSetter, &inputInterface.keyboardKeys,&offScreenManager);

	PostEffectSrCreator::dsvDescriptorHeap = &dsvDescriptorHeap;

	offScreenManager.Init(&textureDataManager, &exclusiveDraw,deviceSetUp.Getter_Device(),
		&commandControl,&swapChainControl,&srvDescHeap);
	
	TimeConstBuffer::Get()->Init(deviceSetUp.Getter_Device());


	return true;
}

LRESULT CALLBACK WinApp::WndProc(HWND hWnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_)
{
#ifdef USE_IMGUI
	//ImGuiにメッセージを渡す
	if (ImGui_ImplWin32_WndProcHandler(hWnd_, msg_, wParam_, lParam_))
	{
		return true;
	}
#endif

	//メッセージに応じてゲーム固有の処理を行う
	switch (msg_)
	{
		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);

		return 0;
	}

	//標準メッセージの処理を行う
	return DefWindowProc(hWnd_, msg_, wParam_, lParam_);

}

void WinApp::TermWnd()
{
	//ウィンドウの登録を解除
	if (m_hInst)
	{
		UnregisterClass(m_windowName, m_hInst);
	}

	m_hInst = nullptr;
	m_hWnd = nullptr;

	inputInterface.Finalize();
}

void WinApp::TermD3D()
{
	CloseHandle(fenceControl.Getter_FenceEvent());

#ifdef USE_IMGUI
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif

	CloseWindow(m_hWnd);

	//COMの終了
	CoUninitialize();

}

WinApp::WinApp(uint32_t width_, uint32_t height_, LPCWSTR windowName_)
	:m_width(width_),
	m_height(height_),
	m_windowName(windowName_),
	m_hInst(nullptr),
	m_hWnd(nullptr)
{
	InitApp();

}

WinApp::~WinApp()
{

}


bool WinApp::InitApp()
{
	// ウィンドウの初期化.
	if (!InitWnd())
	{
		return false;
	}

	// Direct3D 12の初期化.
	if (!InitD3D())
	{
		return false;
	}

	commandControl.Getter_commandList()->Close();
	ID3D12CommandList* commandLists[] = { commandControl.Getter_commandList() };
	commandControl.Getter_CommandQueue()->ExecuteCommandLists(1, commandLists);
	fenceControl.WaitFenceEvent(commandControl.Getter_CommandQueue(), swapChainControl.Getter_SwapChain());

	testMeshShader.Init();

	return true;
}

bool WinApp::InitWnd()
{

#ifdef _DEBUG

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}

#endif // _DEBUG


	auto hInst = GetModuleHandle(nullptr);
	if (!hInst)
	{
		return false;
	}

	// ウィンドウクラスの登録
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WinApp::WndProc;
	wc.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	wc.hCursor = LoadCursor(hInst, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = m_windowName;
	wc.hIconSm = LoadIcon(hInst, IDI_APPLICATION);

	//ウィンドウの登録
	if (!RegisterClassEx(&wc))
	{
		return false;
	}

	//インスタンスハンドルの設定
	m_hInst = hInst;

	//WindowSize
	RECT rc = {};
	rc.right = static_cast<LONG>(m_width);
	rc.bottom = static_cast<LONG>(m_height);
	auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&rc, style, FALSE);

	// ウィンドウの作成
	m_hWnd = CreateWindowEx(
		0,                              // Optional window styles.
		m_windowName,                   // Window class
		m_windowName,					// Window text
		style,							// Window style
		// Size and position
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,							// Parent window    
		nullptr,							// Menu
		m_hInst,						// Instance handle
		nullptr							// Additional application data
	);

	if (m_hWnd == NULL)
	{
		return false;
	}

	// ウィンドウの表示
	ShowWindow(m_hWnd, SW_SHOWNORMAL);

	//ウィンドウにフォーカスを設定
	SetFocus(m_hWnd);

	//入力インターフェースの初期化
	inputInterface.Initialize(m_hInst,m_hWnd);

	return true;
}

void WinApp::TermApp()
{
	TermWnd();
	TermD3D();

}









