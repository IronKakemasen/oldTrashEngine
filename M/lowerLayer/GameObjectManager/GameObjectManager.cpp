#include "GameObjectManager.h"
#include "../GameObject/GameObjectBehavior.h"
#include "../Collision/CollisionDetection/CollisionDetection.h"
#include "../../M.h"

#ifdef USE_IMGUI
#include "imgui.h"
#endif

#ifdef _DEBUG
void GameObjectManager::ForDebug::DrawCollider(GameObject* obj_, Matrix4* vpMat_)
{
	if (collisionVisibility)
	{
		if (!obj_->IsCollisionActivated()) return;

		if (obj_->HasRectCollider())
		{
			Vector3 world = obj_->Getter_Trans()->GetWorldPos();
			Rect r = obj_->Getter_Rect()->ConvertToWorld(world);
			Vector3 LT = { r.left,0,r.top };
			Vector3 RT = { r.right,0,r.top };
			Vector3 RB = { r.right,0,r.bottom };
			Vector3 LB = { r.left,0,r.bottom };

			M::GetInstance()->DrawLine(LT, RT,
				obj_->forDebug.colorForCollision, vpMat_);
			M::GetInstance()->DrawLine(RT, RB,
				obj_->forDebug.colorForCollision, vpMat_);
			M::GetInstance()->DrawLine(RB, LB,
				obj_->forDebug.colorForCollision, vpMat_);
			M::GetInstance()->DrawLine(LB, LT,
				obj_->forDebug.colorForCollision, vpMat_);
		}

		if (obj_->HasCircleCollider())
		{
			Vector3 world = obj_->Getter_Trans()->GetWorldPos();
			Circle* c = obj_->Getter_Circle();

			M::GetInstance()->DrawEllipseWireFrame(world, c->radius,
				{ 90.0f,0,0 }, obj_->forDebug.colorForCollision, vpMat_);
		}

	}
}

GameObjectManager::ForDebug::ForDebug()
{
	collisionVisibility = false;
}

#endif


void GameObjectManager::RegisterForContainer(GameObject* dst_)
{
	dst_->SetGameObjectManager(this);
	dst_->SetNumber(createNum++);
	objContainer.emplace_back(dst_);
}

std::vector<GameObject*> GameObjectManager::Find(GameObject::Tag tag_)
{
	std::vector<GameObject*> data;
	for (auto* i : objContainer)
	{
		if (i->Getter_Identity()->tag == tag_)
		{
			data.emplace_back(i);
		}
	}

	return data;
}

void GameObjectManager::TheWorld()
{
	isStop = !isStop;
}

void GameObjectManager::Debug()
{
#ifdef USE_IMGUI
	static int selec = 0;

	ImGui::Begin("ObjManager", nullptr, ImGuiWindowFlags_MenuBar);
	ImGui::Text("CollisionVisivility : "); ImGui::SameLine();
	ImGui::Checkbox(" ", &forDebug.collisionVisibility);

	if (ImGui::BeginMenuBar()) 
	{
		if (ImGui::BeginMenu("Select Status"))
		{
			if (ImGui::MenuItem("Active")) 
			{
				selec = 0;
			}
			else if (ImGui::MenuItem("InActive"))
			{
				selec = 1;
			}
			else if (ImGui::MenuItem("All"))
			{
				selec = 2; 
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (selec == 0)
	{
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 250), ImGuiWindowFlags_NoTitleBar);

		for (auto itr = objContainer.begin(); itr != objContainer.end(); ++itr)
		{
			if ((*itr) == nullptr) continue;
			else if ((*itr)->GetStatus() == GameObjectBehavior::Status::kInActive) continue;

			if (ImGui::TreeNode((*itr)->Getter_Name().c_str()))
			{
				ImGui::DragFloat3("pos", reinterpret_cast<float*>(&(*itr)->Getter_Trans()->pos), 0.1f);
				ImGui::TreePop();
			}
		}
		ImGui::EndChild();
	}
	else if (selec == 1)
	{
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar);

		for (auto itr = objContainer.begin(); itr != objContainer.end(); ++itr)
		{
			if ((*itr) == nullptr) continue;
			else if ((*itr)->GetStatus() == GameObjectBehavior::Status::kActive) continue;

			if (ImGui::TreeNode((*itr)->Getter_Name().c_str()))
			{
				ImGui::DragFloat3("pos", reinterpret_cast<float*>(&(*itr)->Getter_Trans()->pos), 0.1f);
				ImGui::TreePop();
			}
		}
		ImGui::EndChild();

	}
	else
	{
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar);
		for (auto itr = objContainer.begin(); itr != objContainer.end(); ++itr)
		{
			if ((*itr) == nullptr) continue;

			if (ImGui::TreeNode((*itr)->Getter_Name().c_str()))
			{
				ImGui::DragFloat3("pos", reinterpret_cast<float*>(&(*itr)->Getter_Trans()->pos), 0.1f);
				ImGui::TreePop();
			}
		}
		ImGui::EndChild();
	}

	ImGui::End();

#endif
}
void GameObjectManager::Init()
{
	for (auto itr = objContainer.begin(); itr != objContainer.end(); ++itr)
	{
		(*itr)->Init();
		(*itr)->SetCollisionBackTable();
	}
}

void GameObjectManager::Reset()
{
	for (auto itr = objContainer.begin(); itr != objContainer.end(); ++itr)
	{
		(*itr)->Reset();
	}

	// InGameController取得
	std::vector<GameObject*> inGameControllerList = Find(GameObject::Tag::kInGameController);
	inGameController = reinterpret_cast<InGameController*>(inGameControllerList[0]);
}

void GameObjectManager::Update()
{

	// 登録されたオブジェクトの更新
	for (auto itr = objContainer.begin(); itr != objContainer.end(); ++itr)
	{
		if ((*itr) == nullptr) continue;
		// 非アクティブならスキップ
		else if ((*itr)->GetStatus() != GameObjectBehavior::Status::kActive)
		{
			continue;
		}

		// ポーズ中はInGameControllerとUIManager以外更新しない
		if (isStop)
		{
			if ((*itr)->Getter_Identity()->tag != GameObjectBehavior::Tag::kUIManager &&
				(*itr)->Getter_Identity()->tag != GameObjectBehavior::Tag::kInGameController)
			{
				continue;
			}
		}

		(*itr)->Update();
		ChackAllCollision((*itr));
	}
}

void GameObjectManager::Render(Matrix4* vpMat_)
{
	for (auto itr = objContainer.begin(); itr != objContainer.end(); ++itr)
	{
		if ((*itr) == nullptr) continue;
		else if ((*itr)->GetStatus() == GameObjectBehavior::Status::kInActive)
		{
			continue;
		}

		(*itr)->Draw(vpMat_);

#ifdef _DEBUG
		forDebug.DrawCollider((*itr), vpMat_);
#endif // _DEBUG

	}
}

Vector2 GameObjectManager::ComputeTilePosition(Vector3 wPos_)
{
	static float const invTileLength = 1.0f / tileLength;

	return { (floor(wPos_.x) + 100) * invTileLength,
			(floor(wPos_.z) + 100)* invTileLength };
}

bool GameObjectManager::IsBothPositionsAreNear(Vector3 thisWPos_, Vector3 otherWPos_)
{
	Vector2 obj1 = ComputeTilePosition(thisWPos_);
	Vector2 obj2 = ComputeTilePosition(otherWPos_);

	float absDifX = fabs(obj1.x - obj2.x);
	if (absDifX > 1.0f )
	{
		return false;
	}

	float absDifZ = fabs(obj1.y - obj2.y);
	if (absDifZ > 1.0f)
	{
		return false;
	}

	return true;
}

void GameObjectManager::ChackAllCollision(GameObject* thisObj_)
{
	if (!thisObj_->HasCollider()) return;
	else if (!thisObj_->IsCollisionActivated()) return;

#ifdef _DEBUG
	thisObj_->forDebug.colorForCollision = { 50,50,200,255 };
#endif // _DEBUG

	for (auto* otherObj : objContainer)
	{
		if (!otherObj->HasCollider()) continue;
		else if (!otherObj->IsCollisionActivated()) continue;

		//衝突相手がアクティブでなければ
		if (otherObj->GetStatus() == GameObject::Status::kInActive)
		{
			continue;
		}
		//コライダーを所持しているのか
		else if (!otherObj->HasCollider())
		{
			continue;
		}
		//コリジョンがアクティブ化されているのか
		else if (!otherObj->IsCollisionActivated())
		{
			continue;
		}
		//マスク処理
		else if (thisObj_->IsCollisionMaskMatched(otherObj->Getter_Identity()))
		{
			continue;
		}

		//ワールド座標を取得
		Vector3 thisWorldPos = thisObj_->Getter_Trans()->GetWorldPos();
		Vector3 otherWorldPos = otherObj->Getter_Trans()->GetWorldPos();

		//近くに存在しているのか
		if (!IsBothPositionsAreNear(thisWorldPos, otherWorldPos))
		{
			continue;
		}

		//さーくるコリジョン
		if (thisObj_->HasCircleCollider() && otherObj->HasCircleCollider())
		{ 
			if (CollisionDetections::C2D::CircleCollision(
				thisObj_->Getter_Circle()->radius, thisWorldPos,
				otherObj->Getter_Circle()->radius, otherWorldPos))
			{
				thisObj_->SetCollidedObjPtr(otherObj);
				thisObj_->ActivateOnTriggerEnter(otherObj->Getter_Identity()->tag);
			}
		}
		//クアッドコリジョン
		else if (thisObj_->HasRectCollider() && otherObj->HasRectCollider())
		{
			if (CollisionDetections::C2D::ObjectAABB(
				thisObj_->Getter_Rect(), thisWorldPos,
				otherObj->Getter_Rect(), otherWorldPos))
			{
				thisObj_->SetCollidedObjPtr(otherObj);
				thisObj_->ActivateOnTriggerEnter(otherObj->Getter_Identity()->tag);
			}
		}


	}
}