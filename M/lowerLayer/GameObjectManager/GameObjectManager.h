#pragma once
#include "../GameObject/GameObjectBehavior.h"
#include <vector>

struct Matrix4;
struct InGameController;

class GameObjectManager
{
	std::vector< GameObject* > objContainer;
	int createNum = 0;
	float tileLength = 5.0f;

	//ゲームオブジェクトすべての当たり判定をとる
	void ChackAllCollision(GameObject* thisObj_);
	Vector2 ComputeTilePosition(Vector3 wPos_);
	bool IsBothPositionsAreNear(Vector3 thisWPos_, Vector3 otherWPos_);

#ifdef _DEBUG
	struct ForDebug
	{
		bool collisionVisibility;
		ForDebug();
		void DrawCollider(GameObject* obj_, Matrix4* vpMat_);
	};

	ForDebug forDebug;
#endif // DEBUG

	InGameController* inGameController;

public:
	void Init();
	void Update();
	void Render(Matrix4* vpMat_);
	void Reset();
	void Debug();
	void TheWorld();
	static inline bool isStop = false;

	std::vector<GameObject*> Find(GameObject::Tag tag_);

	template<typename... objPointer>
	void RegisterForContainer(objPointer... pointers_)
	{
		uint8_t length = sizeof...(pointers_);
		GameObject* pointers[] = { pointers_... };

		for (int i = 0; i < length; i++)
		{
			pointers[i]->SetNumber(createNum++);
			pointers[i]->SetGameObjectManager(this);

			objContainer.emplace_back(pointers[i]);
		}
	}

	void RegisterForContainer(GameObject* dst_);


};

