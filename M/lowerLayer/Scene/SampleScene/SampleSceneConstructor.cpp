#include "SampleScene.h"
#include "../../engineCore/Light/DirectionalLight/DirectionalLight.h"
#include "../../engineCore/Light/PointLight/PointLight.h"


SampleScene::SampleScene()
{

}

void SampleScene::Instantiate()
{
	//triangle.reset(new MTriangle);
	//quad.reset(new MQuad);
	//sPlayer.reset(new SamplePlayer);
	sObj.reset(new SampleObj);

	//for (int i = 0; i < 10; ++i)
	//{
	//	sEnemies.emplace_back(std::make_unique<SampleEnemy>());
	//}

}

void SampleScene::Init()
{
	//sPlayer->Init();

	//triangle->Initialize(1.0f, 1.0f, {}, M::GetInstance()->GetTexIndex(TextureTag::kPlayer));
	//quad->Initialize(1.0f, 1.0f, {}, M::GetInstance()->GetTexIndex(TextureTag::kNothing));
	sObj->Init();

	//for (int i = 0; i < 10; ++i)
	//{
	//	sEnemies[i]->Init();
	//}

}
