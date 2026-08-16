#pragma once
#include "../../commonVariables.h"

//=================================================================== debugSystems
#include "./Essential/logAndException/outputDebugLog.h"
#include "./Essential/logAndException/detectException.h"
//===================================================================

//=================================================================== descriptorHeaps
#include "./DescriptorHeap/rtvDescriptorHeap/rtvDescriptorHeap.h"
#include "./DescriptorHeap/dsvDescriptorHeap/dsvDescriptorHeap.h"		
#include "./DescriptorHeap/srvDescriptorHeap/srvDescriptorHeap.h"
//===================================================================

//=================================================================== otherCoreSystems
#include "./Essential/deviceSetUp/deviceSetUp.h"
#include "./Essential/FenceControl/FenceControl.h"
#include "./Essential/CommandControl/CommandControl.h"
#include "./Essential/SwapChainControl/SwapChainControl.h"
#include "./Essential/dxCompile/dxCompile.h"
//===================================================================

//=================================================================== shaderData,Table
#include "./VpShader/vpShaders.h"
//===================================================================

//=================================================================== allPipelineSet,DrawSystem
#include "./PSO/allPipelineSet.h"
#include "./Render/ExclusiveDraw/ExclusiveDraw.h"
#include "./Render/OriginalScreen/OriginalScreen.h"
#include "./Render/Palette/Palette.h"
#include "./Render/OffScreenManager/OffScreenManager.h"
//===================================================================


//=================================================================== allMesh,ParticleMeshSrvCreator
#include "./Mesh/allMesh/allMesh.h"
//===================================================================


//=================================================================== textureSystem
#include "./textureDataManager/textureDataManager.h"
//===================================================================


//=================================================================== MeshCreator
#include "./Mesh/MeshCreator/MeshCreator.h"
//===================================================================


//=================================================================== BufferData,Creator
#include "./Buffer/gpuResources/Data/ShaderBufferData/ShaderBufferData.h"
#include "./Buffer/gpuResources/Creator/SrvCreator/SrvCreator.h"
//===================================================================


//=================================================================== LightManager,LightCreator
#include "./Light/LightManager/LightManager.h"
//===================================================================


//=================================================================== CameraParaBufferSetter
#include "./Buffer/constantBuffer/CameraParaBuffer/CameraParameterSetter/CameraParameterSetter.h"
//===================================================================


//=================================================================== Input
#include "./Input/InputInterface/InputInterface.h"
//===================================================================

#include "TestMeshShader.h"

//=================================================================== Imgui
#ifdef USE_IMGUI
#include "../../../External/imgui/imgui.h"
#include "../../../External/imgui/imgui_impl_dx12.h"
#include "../../../External/imgui/imgui_impl_win32.h"
#endif 
//===================================================================

#include "./FPSControll/FPSController.h"


class WinApp
{
public:

	static inline std::ofstream log;

	WinApp(uint32_t width_, uint32_t height_, LPCWSTR windowName_);
	void TermApp();
	void BeginFrame();
	void EndFrame();
	void OffScreenBegin();
	void OffScreenEnd();


	void TestMeshShaderDraw2(Matrix4* vpMat_);
	void TestMeshShaderDraw(Matrix4* vpMat_);
	void TestMeshShaderBegin();
	void TestMeshShaderEnd();


	~WinApp();
	//[ DeviceSetUp ]
	static inline DeviceSetUp deviceSetUp;
	//[ FenceControl ]
	static inline FenceControl fenceControl;
	//[ DxCompile ]
	static inline DxCompile dxCompile;
	static inline SrvCreator srvCreator;

	TestMeshShader testMeshShader;


private:

	float windowColor[4] = { 0.02f,0.02f,0.02f,1.0f };

#ifdef _DEBUG
	Microsoft::WRL::ComPtr <ID3D12Debug1> debugController = nullptr;
#endif 

	HINSTANCE m_hInst;
	HWND m_hWnd;
	uint32_t m_width;
	uint32_t m_height;
	LPCWSTR m_windowName;


	//[ CommandControl ]
	CommandControl commandControl;


	//[ SwapChain ]
	SwapChainControl swapChainControl;


	//[ DescriptorHeaps ]
	RtvDescriptorHeap rtvDescHeap;
	SrvDescriptorHeap srvDescHeap;
	DsvDescriptorHeap dsvDescriptorHeap;

	//[ ShaderData , vpShadertable  ]
	VpShaders vpShaders;

	//[ AllPipelineSet ]
	AllPipelineSet allPipelineSet;

	//[ ExclusiveDraw ]
	ExclusiveDraw exclusiveDraw;
	OffScreenManager offScreenManager;

	//[ AllMesh ,ParticleMeshSrvCreator ]
	AllMesh allMesh;
	

	//[ ShaderBufferData ]
	ShaderBufferData shaderBufferData;

	//[ TextureDataManager ] 
	TextureDataManager textureDataManager;

	//[ MeshCreator ]
	MeshCreator meshCreator;

	//[ FpsController ]
	FPSController fpsController;

	//[ LightManager ]
	LightManager lightManager;

	//[ CameraParameterSetter ]
	CameraParameterSetter cameraParameterSetter;

	//[ Input ]
	InputInterface inputInterface;

	bool InitApp();
	bool InitWnd();
	void TermWnd();
	bool InitD3D();
	void TermD3D();

	static LRESULT CALLBACK WndProc(HWND hWnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);

};

template<typename T>
void SafeRelease(T*& ptr)
{
	if (ptr != nullptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}

#ifdef USE_IMGUI


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);
#endif // !USE_IMGUI
