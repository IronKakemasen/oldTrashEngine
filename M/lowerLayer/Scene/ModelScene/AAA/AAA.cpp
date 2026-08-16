#include "AAA.h"

//コリジョンバックテーブルを設定
void AAA::SetCollisionBackTable()
{
}



void AAA::Update()
{
	block1->model->GetAppearance(0)->uvTrans.rotate += 1.0f;

}

void AAA::Init()
{
	//モデルの初期化
	block1->Init(&trans);
	c.Initialize(0.75f);


}

void AAA::Reset()
{
	//モデルのリセット（中身が書いてあれば）
	block1->Reset();
}

void AAA::Draw(Matrix4* vpMat_)
{
	//モデルの描画
	block1->Draw(vpMat_);
	M::GetInstance()->DrawEllipseWireFrame(trans.pos, 1.0f, { 0,0,0 }, { 60,120,100,255 }, vpMat_);
	M::GetInstance()->DrawEllipseWireFrame(trans.pos, 1.0f, { 90,0,0 }, { 60,150,100,255 }, vpMat_);
	M::GetInstance()->DrawEllipseWireFrame(trans.pos, 1.0f, { 0,90,0 }, { 60,150,100,255 }, vpMat_);
	M::GetInstance()->DrawEllipseWireFrame(trans.pos, 1.0f, { 0,0,90 }, { 60,150,100,255 }, vpMat_);

}


AAA::AAA()
{
	//モデルのインスタンス化
	block1.reset(new BlockModel);
}
