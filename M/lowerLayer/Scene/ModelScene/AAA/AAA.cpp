#include "AAA.h"
#include "../LevelLoader.h"

//コリジョンバックテーブルを設定
void AAA::SetCollisionBackTable()
{
}



void AAA::Update()
{

}

void AAA::Init()
{
	auto const sceneData = LevelLoader::Load("./resource/preset/levels/modelScene.json");

	//モデルの初期化
	block1->Init(&trans);
	c.Initialize(0.75f);

	trans.pos = sceneData.allObj[0].pos;
	trans.scale = sceneData.allObj[0].scale;
	trans.rotation = sceneData.allObj[0].rotation;

	colliderType = sceneData.allObj[0].colliderType;
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
