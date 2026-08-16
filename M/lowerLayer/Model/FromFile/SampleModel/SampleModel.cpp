#include "SampleModel.h"
#include "../../../../../external/imgui/imgui.h"

void SampleModel::Update(int mode_, float count_)
{

}

void SampleModel::Draw(Matrix4* vpMat_)
{
	M::GetInstance()->DrawModel(model.get(), vpMat_);
}

SampleModel::SampleModel()
{
	////モデルの生成
	//model = M::GetInstance()->CreateModel("./M/resource/preset/model/test/nomalMappedTeapot/teapot.obj");
	////使用するシェーダーの選択
	//(*model->GetAppearance())[0].shaderSetIndex =
	//	M::GetInstance()->GetShaderSetIndexFromFileName("ModelBump.VS", "ModelBump.PS");
	////使用するテクスチャの選択
	//(*model->GetAppearance())[0].SetUsingTextureFromContainer(1, 1, 0, 0);


	////モデルの生成
	//model = M::GetInstance()->CreateModel("./M/resource/preset/model/test/teapot/teapot.obj");
	////使用するシェーダーの選択
	//(*model->GetAppearance())[0].shaderSetIndex =
	//M::GetInstance()->GetShaderSetIndexFromFileName("ModelSimple.VS", "ModelSimple.PS");
	////使用するテクスチャの選択
	//(*model->GetAppearance())[0].SetUsingTextureFromContainer(1, 0, 0, 0);

	////モデルの生成
	//model = M::GetInstance()->CreateModel("./M/resource/preset/model/test/teapot/teapot.obj");
	////使用するシェーダーの選択
	//(*model->GetAppearance())[0].shaderSetIndex =
	//	M::GetInstance()->GetShaderSetIndexFromFileName("ModelPhong.VS", "ModelPhong.PS");
	////使用するテクスチャの選択
	//(*model->GetAppearance())[0].SetUsingTextureFromContainer(1, 0, 0, 0);

	//モデルの生成
	model = M::GetInstance()->CreateModel("./resource/preset/model/test/axis.obj");
	//使用するシェーダーの選択
	(*model->GetAppearance())[0].shaderSetIndex =
		M::GetInstance()->GetShaderSetIndexFromFileName("ModelGGX.VS", "ModelGGX.PS");
	//使用するテクスチャの選択
	(*model->GetAppearance())[0].SetUsingTextureFromContainer(1, 0, 0, 0);


	////モデルの生成
	//model = M::GetInstance()->CreateModel("./M/resource/preset/model/test/teapot/teapot.obj");
	////使用するシェーダーの選択
	//model->Getter_ModelData().appearance[0].shaderSetIndex =
	//	M::GetInstance()->GetShaderSetIndexFromFileName("ModelNormalizedPhong.VS", "ModelNormalizedPhong.PS");
	////使用するテクスチャの選択
	//model->Getter_ModelData().appearance[0].SetUsingTextureFromContainer(1, 0, 0, 0);

}

void SampleModel::Init(Transform* gameObjectTrans_)
{
	(*model->GetAppearance())[0].trans.pos.x = -0.5f;
	(*model->GetAppearance())[0].metalic = 0.72f;
	(*model->GetAppearance())[0].roughness = 0.4f;

}

void SampleModel::Reset()
{

}

