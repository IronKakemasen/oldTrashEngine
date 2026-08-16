#include "AAA.h"

//コリジョンバックテーブルを設定
void AAA::SetCollisionBackTable()
{
}



void AAA::Update()
{
	for (int i = 0; i < 10; ++i)
	{
		block1[i]->model->GetAppearance(0)->color.w *= 0.9875f;
	}
}

void AAA::Init()
{
	//モデルの初期化
	for (int i = 0; i < 10; ++i)block1[i]->Init(&trans);

	c.Initialize(0.75f);


}

void AAA::Reset()
{
	//モデルのリセット（中身が書いてあれば）

}

void AAA::Draw(Matrix4* vpMat_)
{
	//モデルの描画
	for (int i = 0; i < 10; ++i)block1[i]->Draw(vpMat_);
	M::GetInstance()->DrawEllipseWireFrame(trans.pos, 1.0f, {0,0,0}, { 60,120,100,255 }, vpMat_);
	M::GetInstance()->DrawEllipseWireFrame(trans.pos, 1.0f, { 90,0,0 }, { 60,150,100,255 }, vpMat_);
	M::GetInstance()->DrawEllipseWireFrame(trans.pos, 1.0f, { 0,90,0 }, { 60,150,100,255 }, vpMat_);
	M::GetInstance()->DrawEllipseWireFrame(trans.pos, 1.0f, { 0,0,90 }, { 60,150,100,255 }, vpMat_);

}


AAA::AAA()
{
	//モデルのインスタンス化
	for (int i = 0; i < 10; ++i)
	{
		block1.emplace_back(std::make_unique<BlockModel>());
	}
}
