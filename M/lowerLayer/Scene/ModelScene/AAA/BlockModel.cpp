#include "BlockModel.h"

BlockModel::BlockModel()
{
	//モデルの生成(必須)
	model = M::GetInstance()->CreateModel("./resource/preset/model/Plane/Plane.obj");

}

void BlockModel::Update(int mode_, float count_)
{


}

void BlockModel::Draw(Matrix4* vpMat_)
{
	M::GetInstance()->DrawModel(model.get(), vpMat_);
}

void BlockModel::Init(Transform* gameObjectTrans_)
{
	//↓↓↓↓↓必須↓↓↓↓↓

	//見た目のパラメーター
	//複数モデルを考慮しているためインデックスで指定する
	auto* appearance = model->GetAppearance(0);
	//使用するシェーダーの選択
	appearance->shaderSetIndex =
		M::GetInstance()->GetShaderSetIndexFromFileName("ModelGGX.VS", "ModelGGX.PS");

	appearance->texHandlesContainer[Appearance::kColormap] =
		M::GetInstance()->GetTexIndex(TextureTag::kSouhei);
	//使用するテクスチャ種類の選択(カラーマップ、ノーマルマップ、...)
	appearance->SetUsingTextureFromContainer(1, 0, 0, 0);

	//ゲームオブジェクトと全モデルのペアレント化
	MakeAllPartsBeChildren(gameObjectTrans_);

	//↑↑↑↑↑必須↑↑↑↑↑

	//必須でない
	appearance->metalic = 0.85f;
	appearance->roughness = 0.4f;
	appearance->color = { 255,255,255,255 };
	appearance->trans.scale = { 3,3,3};
	appearance->trans.rotation = { -90,0,0 };
}

void BlockModel::Reset()
{
}
