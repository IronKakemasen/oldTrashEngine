#include "ModelScene.h"

ModelScene::ModelScene()
{
	aaa.reset(new AAA);
	gameObjManager->RegisterForContainer(aaa.get());



	dirLight->Getter_Para()->intensity = 1.0f;
}

void ModelScene::Update()
{

}

void ModelScene::Draw()
{

}

void ModelScene::Reset()
{

}

void ModelScene::Debug()
{

}

void ModelScene::Instantiate()
{

}

void ModelScene::Init()
{

}