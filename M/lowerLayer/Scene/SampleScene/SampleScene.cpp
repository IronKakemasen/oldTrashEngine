#include "SampleScene.h"
#include "../../../../external/imgui/imgui.h"
#include "../../engineCore/Light/DirectionalLight/DirectionalLight.h"
#include "../../engineCore/Light/PointLight/PointLight.h"


void SampleScene::Update()
{	
	//sPlayer->Update();
	sObj->Update();
}

void SampleScene::Draw()
{
	Matrix4* vpMat = &cameraController->GetUsingCamera()->vpMat;
	Matrix4 m = Get_Orthographic3D(0.0f, CommonV::kWindow_W, 0.0f, CommonV::kWindow_H);

	//sPlayer->Draw(vpMat);
	sObj->Draw(vpMat);
}

void SampleScene::Reset()
{

}

void SampleScene::Debug()
{
#ifdef USE_IMGUI

	Vector3 from0 = Vector3{ 1,0.7f,0.5f }.GetNormalized();
	Vector3 t0 = from0 * -1.0f;

	Vector3 from1 = Vector3{ -0.6f,0.9f,0.2f}.GetNormalized();
	Vector3 t1 = Vector3{ 0.4f,0.7f,-0.5f}.GetNormalized();

	static Matrix4 m[3];

	m[0] = GetRotateAxisMat(Vector3{ 1,0,0 }, Vector3{ -1,0,0 });
	m[1] = GetRotateAxisMat(from0, t0);
	m[2] = GetRotateAxisMat(from1, t1);

	ImGui::Begin("kadai");

	for (int i = 0; i < 3; ++i)
	{
		ImGui::Text(std::to_string(i).c_str());
		for (int k = 0; k < 4; ++k)
		{
			ImGui::DragFloat4(("m" + std::to_string(k)).c_str(),
				reinterpret_cast<float*>(&m[i].m[k]));

		}

	}
	ImGui::End();



	ImGui::Begin("oh");

	if (ImGui::TreeNode("obj"))
	{
		ImGui::DragFloat3("pos", reinterpret_cast<float*>(&sObj->Getter_Trans()->pos), 0.1f);
		ImGui::DragFloat3("rotate", reinterpret_cast<float*>(&sObj->Getter_Trans()->rotation), 0.1f);
		ImGui::DragFloat3("scale", reinterpret_cast<float*>(&sObj->Getter_Trans()->scale), 0.1f);

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("camera"))
	{
		auto* cur_camera = cameraController->GetUsingCamera();
		auto* para = cur_camera->Getter_Parameters();
		ImGui::DragFloat3("pos", reinterpret_cast<float*>(&para->trans.pos), 0.1f);
		ImGui::DragFloat3("tragerDir", reinterpret_cast<float*>(&para->trans.lookDir), 0.025f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("dirLight"))
	{
		auto* para = dirLight->Getter_Para();
		ImGui::DragFloat3("pos", reinterpret_cast<float*>(&para->pos), 0.1f);
		ImGui::DragFloat3("color", reinterpret_cast<float*>(&para->color), 0.1f);
		ImGui::DragFloat("intensity", reinterpret_cast<float*>(&para->intensity), 0.025f);
		ImGui::Checkbox("isActive", reinterpret_cast<bool*>(&para->isActive));

		ImGui::TreePop();
	}
	ImGui::End();


#endif // USE_IMGUI

}

