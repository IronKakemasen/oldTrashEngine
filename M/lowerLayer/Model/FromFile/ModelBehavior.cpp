#include "ModelBehavior.h"

ModelBehavior::ModelBehavior()
{
}

void ModelBehavior::MakeAllPartsBeChildren(Transform* gameObject_)
{
	auto appearance = model->GetAppearance()->begin();
	for (; appearance != model->GetAppearance()->end(); ++appearance)
	{
		appearance->trans.BeChildren(gameObject_);
	}
}
