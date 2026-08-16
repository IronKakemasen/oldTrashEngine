#include "M.h"
#include "./engineCore/Render/ExclusiveDraw/ExclusiveDraw.h"
#include "./engineCore/WinApp.h"
#include "./engineCore/VpShader/vpShaderTable/vpShaderTable.h"
#include "./engineCore/PSO/allPipelineSet.h"
#include "./engineCore/Light/LightManager/LightManager.h"
#include "./engineCore/Buffer/constantBuffer/CameraParaBuffer/CameraParameterSetter/CameraParameterSetter.h"
#include "./engineCore/Render/OffScreenManager/OffScreenManager.h"

float M::GetDeltaTime()
{
	return 0.0166666666f;
}

bool M::IsKeyTriggered(KeyType key_)
{
	return keyboardKeys->IsKeyTrigger(key_);
}

bool M::IsKeyPressed(KeyType key_)
{
	return keyboardKeys->IsKeyPress(key_);
}

void M::SetCameraBufferPara(CameraBufferPara CameraBufferPara_)
{
	cameraParameterSetter->SetCameraPara(CameraBufferPara_);
}

void M::ChangePostEffect(PostEffectType type_)
{
    offScreenManager->ChangePostEffection(type_);
}

PostEffectType M::WatchEffectType()
{
    return offScreenManager->WatchEffectType();
}

DirectionalLight* M::ImportDirLight()
{
	return lightManager->ExportDirectionalLight();
}

PointLight* M::ImportPointLight()
{
	return lightManager->ExportPointLight();
}

RectLight* M::ImportRectLight()
{
    return lightManager->ExportRectLight();

}



std::unique_ptr<ModelSimple> M::CreateModel(std::string filePath_)
{
	return std::move(meshCreator->CreateModel(filePath_));
}

void M::DrawModel(MeshAndDataCommon* meshAndData_, Matrix4* vpMat_)
{
	exclusiveDraw->DrawModel(meshAndData_, vpMat_);
}

void M::DrawShaderToy(MeshAndDataCommon* meshAndData_, Matrix4* vpMat_)
{
    exclusiveDraw->DrawShaderToy(meshAndData_, vpMat_, passedTime);
}


void M::DrawInstancingParticle2D(int numParticles_, Vector4 color_, int texHandle_,
	BlendMode blendMode_, CullMode cullMode_, int shaderSet_,
	std::vector<Transform> trans_, UVTransform* uvTrans_, Matrix4* vpMat_)
{
	exclusiveDraw->DrawInstancingParticle2D(numParticles_, color_, texHandle_,
		blendMode_, cullMode_, shaderSet_,
		trans_, uvTrans_, vpMat_);
}

void M::DrawMTriangle(Vertex& left_, Vertex& top_, Vertex& right_,
	Vector4 color_, int texHandle_, DrawMode drawMode_, BlendMode blendMode_, CullMode cullMode_,int shaderSet_,
	Transform& trans_, UVTransform& uvTrans_, Matrix4& vpMat_)
{
	exclusiveDraw->DrawMobileTriangle(left_, top_, right_,
		color_, texHandle_, drawMode_, blendMode_, cullMode_, shaderSet_,
		trans_, uvTrans_, vpMat_);
}

void M::DrawMQuad(Vertex& leftTop_, Vertex& rightTop_, Vertex& rightBottom_, Vertex& leftBottom_,
	Vector4 color_, int texHandle_, DrawMode drawMode_, BlendMode blendMode_, CullMode cullMode_, int shaderSet_,
	Transform& trans_, UVTransform& uvTrans_, Matrix4& vpMat_)
{
	exclusiveDraw->DrawMobileQuad(leftTop_, rightTop_, rightBottom_, leftBottom_,
		color_, texHandle_, drawMode_, blendMode_, cullMode_, shaderSet_,
		trans_, uvTrans_, vpMat_);
}

void M::DrawSprite(Vertex& leftTop_, Vertex& rightTop_, Vertex& rightBottom_, Vertex& leftBottom_,
	Vector4 color_, int texHandle_, DrawMode drawMode_, BlendMode blendMode_, CullMode cullMode_, int shaderSet_,
	Transform& trans_, UVTransform& uvTrans_, Matrix4& vpMat_)
{
	exclusiveDraw->DrawSprite(leftTop_, rightTop_, rightBottom_, leftBottom_,
		color_, texHandle_, drawMode_, blendMode_, cullMode_, shaderSet_,
		trans_, uvTrans_, vpMat_);
}

void M::DrawLine(Vector3 st_, Vector3 ed_, Vector4 color_, Matrix4* vpMat_)
{
	exclusiveDraw->DrawLine(st_, ed_, color_, vpMat_);
}

void M::DrawEllipseWireFrame(Vector3 center_, float radius_, Vector3 rotation_,
	Vector4 color_, Matrix4* vpMat_)
{
	exclusiveDraw->DrawEllipseWireFrame(center_, radius_, rotation_,
		color_, vpMat_);
}


M* M::GetInstance()
{
	static M m;
	return &m;
}

int M::GetTexIndex(TextureTag tag_)
{
	return resourceContainer.GetTextureIndex(tag_);
}

int M::GetShaderSetIndexFromFileName(std::string vertexShader_, std::string pixelShader_)
{
	return vpShaderTable->GetIDFromTableName(vertexShader_ + " + " +  pixelShader_);
}

void M::Init(TextureDataManager* textureDataManager_, ExclusiveDraw* exclusiveDraw_, 
	VPShaderTable* vpShaderTable_, AllPipelineSet* allPipelineSet_, MeshCreator* meshCreator_,
	LightManager* lightManager_, CameraParameterSetter* cameraParameterSetter_,
	KeyboardKeys* keyboardKeys_, OffScreenManager* offScreenManager_)
{
	//一度だけ初期化
	static bool initOnlyOnce = true;

    if (!initOnlyOnce)return;
    initOnlyOnce = false;

    resourceContainer.Init(textureDataManager_);
    offScreenManager =  offScreenManager_;
    exclusiveDraw = exclusiveDraw_;
    vpShaderTable = vpShaderTable_;
    allPipelineSet = allPipelineSet_;
    meshCreator = meshCreator_;
    lightManager = lightManager_;
    cameraParameterSetter = cameraParameterSetter_;
    keyboardKeys = keyboardKeys_;

}

void M::LogM(std::string message_)
{
	Log(WinApp::log, message_);
}



M::GetPadState::GetPadState()
{
    for (int32_t i = 0; i < 4; ++i)
    {
        isConnect[i] = false;
    }

    {
        // 明示的初期化
        std::memset(state, 0, sizeof(state));
        std::memset(preState, 0, sizeof(preState));
        std::memset(isConnect, 0, sizeof(isConnect));
        std::memset(leftTrigger, 0, sizeof(leftTrigger));
        std::memset(rightTrigger, 0, sizeof(rightTrigger));
        std::memset(leftStickX, 0, sizeof(leftStickX));
        std::memset(leftStickY, 0, sizeof(leftStickY));
        std::memset(rightStickX, 0, sizeof(rightStickX));
        std::memset(rightStickY, 0, sizeof(rightStickY));
        for (int i = 0; i < 4; ++i)
        {
            leftStickDir[i] = { 0.0f, 0.0f };
            rightStickDir[i] = { 0.0f, 0.0f };
            for (int b = 0; b < PAD_BUTTON_MAX; ++b)
            {
                padStates[i][b].curr = false;
                padStates[i][b].prev = false;
                padStates[i][b].holdFrames = 0;
                padStates[i][b].lastHoldOnRelease = 0;
            }
        }
    }
}

M::GetPadState::~GetPadState()
{}

void M::GetPadState::Update()
{
    for (DWORD i = 0; i < 4; ++i)
    {
        preState[i] = state[i];

        ZeroMemory(&state[i], sizeof(XINPUT_STATE));
        if (XInputGetState(i, &state[i]) == ERROR_SUCCESS)
        {
            for (int32_t button = 0; button < PAD_BUTTON_MAX; ++button)
            {
                padStates[i][button].prev = padStates[i][button].curr;

                if (padStates[i][button].curr)
                {
                    ++padStates[i][button].holdFrames;
                }
                else
                {
                    if (padStates[i][button].prev)
                    {
                        padStates[i][button].lastHoldOnRelease = padStates[i][button].holdFrames;
                    }
                    padStates[i][button].holdFrames = 0;
                }
            }

            WORD b = state[i].Gamepad.wButtons;
            padStates[i][PAD_A].curr = (b & XINPUT_GAMEPAD_A) != 0;
            padStates[i][PAD_B].curr = (b & XINPUT_GAMEPAD_B) != 0;
            padStates[i][PAD_X].curr = (b & XINPUT_GAMEPAD_X) != 0;
            padStates[i][PAD_Y].curr = (b & XINPUT_GAMEPAD_Y) != 0;
            padStates[i][PAD_LB].curr = (b & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
            padStates[i][PAD_RB].curr = (b & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
            padStates[i][PAD_LS].curr = (b & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
            padStates[i][PAD_RS].curr = (b & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;
            padStates[i][PAD_BACK].curr = (b & XINPUT_GAMEPAD_BACK) != 0;
            padStates[i][PAD_START].curr = (b & XINPUT_GAMEPAD_START) != 0;
            padStates[i][PAD_UP].curr = (b & XINPUT_GAMEPAD_DPAD_UP) != 0;
            padStates[i][PAD_DOWN].curr = (b & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
            padStates[i][PAD_LEFT].curr = (b & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
            padStates[i][PAD_RIGHT].curr = (b & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;

            leftTrigger[i] = state[i].Gamepad.bLeftTrigger;
            rightTrigger[i] = state[i].Gamepad.bRightTrigger;

            leftStickX[i] = state[i].Gamepad.sThumbLX;
            leftStickY[i] = state[i].Gamepad.sThumbLY;
            rightStickX[i] = state[i].Gamepad.sThumbRX;
            rightStickY[i] = state[i].Gamepad.sThumbRY;

            Vector2 leftStick = { float(leftStickX[i]), float(leftStickY[i]) };
            leftStickDir[i] = Vector2(leftStick.x / 32768.0f, leftStick.y / 32768.0f);
            Vector2 rightStick = { float(rightStickX[i]), float(rightStickY[i]) };
            rightStickDir[i] = Vector2(rightStick.x / 32768.0f, rightStick.y / 32768.0f);
            isConnect[i] = true;

        }
        else
        {
            isConnect[i] = false;
            for (int button = 0; button < PAD_BUTTON_MAX; ++button)
            {
                PadButtonState& ps = padStates[i][button];
                ps.prev = ps.curr;
                ps.curr = false;
                if (ps.prev) ps.lastHoldOnRelease = ps.holdFrames;
                ps.holdFrames = 0;
            }
            leftTrigger[i] = rightTrigger[i] = 0;
            leftStickX[i] = leftStickY[i] = 0;
            rightStickX[i] = rightStickY[i] = 0;
            leftStickDir[i] = { 0.0f, 0.0f };
            rightStickDir[i] = { 0.0f, 0.0f };

        }
    }
}

bool M::GetPadState::IsHeld(int padIndex, BYTE button)
{
    int stateIndex = std::clamp(padIndex, 0, 3);
    int btn = std::clamp(int(button), 0, PAD_BUTTON_MAX - 1);
    return padStates[stateIndex][btn].curr;
}

bool M::GetPadState::IsJustPressed(int padIndex, BYTE button)
{
    int stateIndex = std::clamp(padIndex, 0, 3);
    int btn = std::clamp(int(button), 0, PAD_BUTTON_MAX - 1);
    return (!padStates[stateIndex][btn].prev && padStates[stateIndex][btn].curr);
}

bool M::GetPadState::IsJustReleased(int padIndex, BYTE button)
{
    int stateIndex = std::clamp(padIndex, 0, 3);
    int btn = std::clamp(int(button), 0, PAD_BUTTON_MAX - 1);
    return (padStates[stateIndex][btn].prev && !padStates[stateIndex][btn].curr);
}

int32_t M::GetPadState::HoldFrames(int padIndex, BYTE button)
{
    int stateIndex = std::clamp(padIndex, 0, 3);
    int btn = std::clamp(int(button), 0, PAD_BUTTON_MAX - 1);
    return padStates[stateIndex][btn].holdFrames;
}

Vector2 M::GetPadState::GetLeftStick(int padIndex)
{
    int stateIndex = std::clamp(padIndex, 0, 3);
    return leftStickDir[stateIndex];
}

Vector2 M::GetPadState::GetRightStick(int padIndex)
{
    int stateIndex = std::clamp(padIndex, 0, 3);
    return rightStickDir[stateIndex];
}

float M::GetPadState::GetLeftTrigger(int padIndex)
{
    int stateIndex = std::clamp(padIndex, 0, 3);
    float value = static_cast<float>(leftTrigger[stateIndex]) / 255.0f;
    return value;
}

float M::GetPadState::GetRightTrigger(int padIndex)
{
    int stateIndex = std::clamp(padIndex, 0, 3);
    float value = static_cast<float>(rightTrigger[stateIndex]) / 255.0f;
    return value;
}

void M::GetPadState::SetVibration(int padIndex, float leftMotor, float rightMotor)
{
    int stateIndex = std::clamp(padIndex, 0, 3);

    leftMotor = std::clamp(leftMotor, 0.0f, 1.0f);
    rightMotor = std::clamp(rightMotor, 0.0f, 1.0f);

    XINPUT_VIBRATION vib;
    ZeroMemory(&vib, sizeof(vib));
    // XInput uses 0..65535 for motor speed
    vib.wLeftMotorSpeed = static_cast<WORD>(leftMotor * 65535.0f);
    vib.wRightMotorSpeed = static_cast<WORD>(rightMotor * 65535.0f);

    XInputSetState(static_cast<DWORD>(stateIndex), &vib);
}

int32_t M::GetPadState::GetConnectedPadNum() const
{
    int sum = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (isConnect[i])
        {
            ++sum;
        }
    }
    return sum;
}
