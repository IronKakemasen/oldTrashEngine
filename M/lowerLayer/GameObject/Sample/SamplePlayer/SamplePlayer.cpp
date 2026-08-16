#include "SamplePlayer.h"

void SamplePlayer::Update()
{

}

void SamplePlayer::Init()
{
	trans.pos = {-3.0f,0.0f,0.0f};
}

void SamplePlayer::Reset()
{

}

void SamplePlayer::Draw(Matrix4* vpMat_)
{
	quadModel.Draw(vpMat_);
}


SamplePlayer::SamplePlayer()
{
	quadModel.appearance.trans.parent = &trans;
	quadModel.Initialize(0.5f, 0.5f, {}, M::GetInstance()->GetTexIndex(TextureTag::kWhite2x2),
		{ 255,255,255,255});
	quadModel.appearance.trans.rotation.x = 45.0f;
	quadModel.appearance.roughness = 0.0f;
	quadModel.appearance.metalic = 0.0f;

}

void SamplePlayer::SetCollisionBackTable()
{

}
