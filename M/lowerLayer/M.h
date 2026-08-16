#pragma once
#include "./engineCore/PSO/pipelineCreators/pipelineComponents.h"
#include "./ResourceContainer/ResourceContainer.h"
#include "../utilities/Transform/Transform.h"
#include "../utilities/UVTransform/UVTransform.h"
#include "./engineCore/Mesh/meshStructure/Vertex/Vertex.h"
#include "./engineCore/Mesh/MeshCreator/MeshCreator.h"
#include "./engineCore/Input/InputInterface/InputInterface.h"
#include "./engineCore/Render/Palette/PostEffectType.h"

#include <functional>
#include <d3d12.h>
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

#define PAD_A               0x00
#define PAD_B               0x01
#define PAD_X               0x02
#define PAD_Y               0x03
#define PAD_LB              0x04
#define PAD_RB              0x05
#define PAD_LS              0x06
#define PAD_RS              0x07
#define PAD_BACK            0x08
#define PAD_START           0x09
#define PAD_UP              0x0A
#define PAD_DOWN            0x0B
#define PAD_LEFT            0x0C
#define PAD_RIGHT           0x0D
#define PAD_BUTTON_MAX      0x0E


class TextureDataManager;
class ExclusiveDraw;
struct CommonDrawingSystem;
class VPShaderTable;
class AllPipelineSet;
struct MeshAndDataCommon;
class LightManager;
class PointLight;
class RectLight;
class CameraParameterSetter;
struct CameraBufferPara;
class DirectionalLight;
class KeyboardKeys;
class OffScreenManager;

class M
{
	OffScreenManager* offScreenManager = nullptr;
	ResourceContainer resourceContainer;
	ExclusiveDraw* exclusiveDraw = nullptr;
	VPShaderTable* vpShaderTable = nullptr;
	AllPipelineSet* allPipelineSet = nullptr;
	MeshCreator* meshCreator = nullptr;
	LightManager* lightManager = nullptr;
	CameraParameterSetter* cameraParameterSetter = nullptr;
	KeyboardKeys* keyboardKeys = nullptr;

public:
	float passedTime = 0.0f;

	static M* GetInstance();

	void Init(TextureDataManager* textureDataManager_, ExclusiveDraw* exclusiveDraw_, 
		VPShaderTable* vpShaderTable_, AllPipelineSet* allPipelineSet_, MeshCreator* meshCreator_,
		LightManager* lightManager_, CameraParameterSetter* cameraParameterSetter_,
		KeyboardKeys* keyboardKeys_, OffScreenManager* offScreenManager_);

	float GetDeltaTime();
	//入力
	bool IsKeyTriggered(KeyType key_);
	bool IsKeyPressed(KeyType key_);
	//テクスチャのインデックスを得る
	int GetTexIndex(TextureTag tag_);
	//モデルの生成
	std::unique_ptr<ModelSimple> CreateModel(std::string filePath_);
	DirectionalLight* ImportDirLight();
	PointLight* ImportPointLight();
	RectLight* ImportRectLight();

	//Logを出力
	void LogM(std::string message_);

	//shaderSetのインデックスをストリングで取得
	int GetShaderSetIndexFromFileName(std::string vertexShader_, std::string pixelShader_);

	//モデルの描画
	void DrawModel(MeshAndDataCommon* meshAndData_, Matrix4* vpMat_);
	void DrawShaderToy(MeshAndDataCommon* meshAndData_, Matrix4* vpMat_);

	//モバイルトライアングルの描画
	void DrawMTriangle(Vertex& left_, Vertex& top_, Vertex& right_,
		Vector4 color_, int texHandle_, DrawMode drawMode_, BlendMode blendMode_, CullMode cullMode_, int shaderSet_,
		Transform& trans_, UVTransform& uvTrans_, Matrix4& vpMat_);

	//モバイルクアッドの描画
	void DrawMQuad(Vertex& leftTop_, Vertex& rightTop_, Vertex& rightBottom_, Vertex& leftBottom_,
		Vector4 color_, int texHandle_, DrawMode drawMode_, BlendMode blendMode_, CullMode cullMode_, int shaderSet_,
		Transform& trans_, UVTransform& uvTrans_, Matrix4& vpMat_);

	//モバイルクアッドの描画
	void DrawSprite(Vertex& leftTop_, Vertex& rightTop_, Vertex& rightBottom_, Vertex& leftBottom_,
		Vector4 color_, int texHandle_, DrawMode drawMode_, BlendMode blendMode_, CullMode cullMode_, int shaderSet_,
		Transform& trans_, UVTransform& uvTrans_, Matrix4& vpMat_);

	void DrawLine(Vector3 st_, Vector3 ed_, Vector4 color_, Matrix4* vpMat_);
	void DrawEllipseWireFrame(Vector3 center_, float radius_, Vector3 rotation_,
		Vector4 color_, Matrix4* vpMat_);
	void DrawInstancingParticle2D(int numParticles_, Vector4 color_, int texHandle_,
		BlendMode blendMode_, CullMode cullMode_, int shaderSet_,
		std::vector<Transform> trans_, UVTransform* uvTrans_, Matrix4* vpMat_);

	void SetCameraBufferPara(CameraBufferPara cameraBufferPara_);
	
	void ChangePostEffect(PostEffectType type_);
	PostEffectType WatchEffectType();
private:

	M() = default;
	~M() = default;

	// コピー・ムーブ禁止
	M(const M&) = delete;
	M& operator=(const M&) = delete;



private:
	class GetPadState
	{
		struct PadButtonState
		{
			bool curr = false;           // 今フレームの押下状態
			bool prev = false;           // 前フレームの押下状態
			int32_t holdFrames = 0;     // 長押しフレーム数
			int32_t lastHoldOnRelease = 0; // 直近のリリース時に押されていたフレーム数
		};

	public:
		GetPadState();
		~GetPadState();

		void Update();

		bool IsHeld(int padIndex, BYTE button);
		bool IsJustPressed(int padIndex, BYTE button);
		bool IsJustReleased(int padIndex, BYTE button);

		int32_t HoldFrames(int padIndex, BYTE button);	// 押されてからの経過フレーム数
		Vector2 GetLeftStick(int padIndex);	    // 左スティックの値取得 (-1.0f ～ 1.0f)
		Vector2 GetRightStick(int padIndex);	// 右スティックの値取得 (-1.0f ～ 1.0f)

		// トリガーの値取得（0.0f ～ 1.0f）
		float GetLeftTrigger(int padIndex);
		float GetRightTrigger(int padIndex);
		// ゲームパッド振動
		void SetVibration(int padIndex, float leftMotor, float rightMotor);

		int32_t GetConnectedPadNum() const;

	private:
		PadButtonState padStates[4][PAD_BUTTON_MAX]{};

		bool isConnect[4];

		BYTE leftTrigger[4];     // 左トリガー（0〜255）
		BYTE rightTrigger[4];    // 右トリガー（0〜255）

		SHORT leftStickX[4];        // 左スティックX軸（-32768〜32767）
		SHORT leftStickY[4];        // 左スティックY軸（-32768〜32767）
		SHORT rightStickX[4];        // 右スティックX軸（-32768〜32767）
		SHORT rightStickY[4];        // 右スティックY軸（-32768〜32767）
		Vector2 leftStickDir[4];        // 左スティック方向({-1〜1},{-1〜1})
		Vector2 rightStickDir[4];        // 右スティック方向({-1〜1},{-1〜1})

		XINPUT_STATE state[4];
		XINPUT_STATE preState[4];
		DWORD dwUserIndex[4];
	};

public:


	GetPadState getPadState;
};

