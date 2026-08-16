#include "SampleEnemy.h"

void SampleEnemy::Update()
{

}

void SampleEnemy::Init()
{

}

void SampleEnemy::Reset()
{

}

void SampleEnemy::Draw(Matrix4* vpMat_)
{
	triangleModel.Draw(vpMat_);
}

SampleEnemy::SampleEnemy()
{
	triangleModel.appearance.trans.parent = &trans;

}

void SampleEnemy::SetCollisionBackTable()
{

}
