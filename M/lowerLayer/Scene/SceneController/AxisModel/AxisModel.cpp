#include "AxisModel.h"

AxisModel::AxisModel()
{
	//モデルの生成(必須)
	model = M::GetInstance()->CreateModel("./resource/preset/model/axis/axis.obj");

}

void AxisModel::Update(int mode_ , float count_ )
{

}

void AxisModel::Draw(Matrix4* vpMat_)
{
	//M::GetInstance()->DrawModel(model.get(), vpMat_);
}

void AxisModel::Init(Transform* gameObjectTrans_)
{

	//見た目のパラメーター
	for (auto appearance = model->GetAppearance()->begin();
		appearance != model->GetAppearance()->end(); ++appearance)
	{
		//使用するシェーダーの選択
		appearance->shaderSetIndex =
			M::GetInstance()->GetShaderSetIndexFromFileName("ModelGGX.VS", "ModelGGX.PS");

		//使用するテクスチャ種類の選択(カラーマップ、ノーマルマップ、...)
		appearance->SetUsingTextureFromContainer(1, 0, 0, 0);

		//ゲームオブジェクトと全モデルのペアレント化
		MakeAllPartsBeChildren(gameObjectTrans_);

		//↑↑↑↑↑必須↑↑↑↑↑

		//必須でない
		appearance->metalic = 0.85f;
		appearance->roughness = 0.4f;
		appearance->trans.pos = { 20,10,15 };
	}

	model->GetAppearance(0)->color = { 0,50,200,255 };
	model->GetAppearance(1)->color = { 200,0,50,255 };
	model->GetAppearance(2)->color = { 0,200,50,255 };
	model->GetAppearance(3)->color = { 200,200,100,255 };

}

void AxisModel::Reset()
{

}
