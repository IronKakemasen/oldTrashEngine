#include "TestParticle.h"


TestParticle::TestParticle()
{
	SetNumParticle(10);
}

void TestParticle::Update()
{

}

void TestParticle::Draw(Matrix4* vpMat_)
{
	M::GetInstance()->DrawInstancingParticle2D(Watch_Numparticles(), color,
		texHandle, blendMode, cullMode, shaderSetIndex, trans, &uvTrans, vpMat_);
}

void TestParticle::Init()
{
	for (int i = 0; i < numParticle; ++i)
	{
		trans[i].translate = { float(i)*0.1f,float(i) * 0.1f,0.0f };
	}

	shaderSetIndex = M::GetInstance()->
		GetShaderSetIndexFromFileName("TestParticle2D.VS", "TestParticle2D.PS");

	texHandle = M::GetInstance()->GetTexIndex(TextureTag::kPlayer);
}
