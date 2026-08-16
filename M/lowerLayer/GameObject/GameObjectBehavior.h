#pragma once
#include "../../utilities/Transform/Transform.h"
#include "../../utilities/Counter/Counter.h"
#include "../../utilities/Easing/EasingFunctions.h"
#include "../../utilities/ConvertToRadian/ConvertToRadian.h"
#include "../Collision/Shape/Rect.h"
#include "../Collision/Shape/Circle.h"

#include <string>
#include <memory>


class GameObjectManager;


struct GameObjectBehavior
{
	//オブジェクトのステータス
	enum class Status
	{
		kActive,	//更新や描画処理します
		kDrawOnly,  //描画オンリー
		kInActive,	//しません
	};

	//ゲームオブジェクトのタグ。適宜追加
	enum Tag
	{
		kEnemy,
		kEnemyFactory,
		kEnemyTower,

		kPlayer,
		kPlayerBullet,
		kPlayerTower,
		kPlayerAlly,

		kInGameController,
		kUIManager,

		kCount,
		kNone,
	};
private :
	struct Identity
	{
		std::string name;
		Tag tag;
		int number;
		//自身の衝突ナンバー
		uint32_t collisionAttribute;
		//衝突可能相手のナンバー
		uint32_t collisionMask;

		Identity();
		Identity(std::string name_, Tag tag_, int number_, uint32_t att_, uint32_t mask_);
		bool IsCollisionMaskMatched(Identity* other_);

	};

	struct Collision
	{
		std::function<void()> collisionBack;
		//矩形コリジョン
		std::unique_ptr <Rect> rect;
		//円形コリジョン
		std::unique_ptr <Circle> circle;
		//各衝突相手に対して衝突後の処理（バック）を設定するための箱
		std::unordered_map<Tag, std::function<void()>> collisionBackActivationMap;
		//衝突相手のポインタ
		GameObjectBehavior* colObj;
		bool collisionActivate = true;
	};

	Identity identity;
	Collision collision;

protected:
	GameObjectManager* gameObjectManager = nullptr;

public:

	Status status;
	Transform trans;

#ifdef _DEBUG
	struct ForDebug
	{
		Vector4 colorForCollision = { 50,50,200,255 };
	};

	ForDebug forDebug;

#endif // _DEBUG

	virtual void Update() = 0;
	virtual void Init() = 0;
	virtual void Reset() = 0;
	virtual void Draw(Matrix4* vpMat_) = 0;
	virtual void SetCollisionBackTable() = 0;

	void SetStatus(Status dst_);
	void SetIdentity(Tag tag_);
	void SetRectCollision(float width_, float height_, Vector3 centerPos_ = {});
	void SetCircleCollision(float radius_);
	void SetCollisionBack(Tag tag_, std::function<void()> func_);

	void SwitchCollisionActivation(bool bool_);

	void SetNumber(int id_);
	void ActivateOnTriggerEnter(Tag tag_);
	bool IsCollisionMaskMatched(Identity* other_);
	Identity* Getter_Identity();
	bool HasRectCollider();
	bool HasCircleCollider();
	bool HasCollider();
	std::string Getter_Name();
	bool IsCollisionActivated();
	void SetCollidedObjPtr(GameObjectBehavior* obj_);
	GameObjectBehavior* Getter_ColObj();
	Rect* Getter_Rect();
	Circle* Getter_Circle();
	void SetGameObjectManager(GameObjectManager* gameObjectManager_);

};

struct GameObject:public GameObjectBehavior
{
public:

	GameObject();
	virtual void Update()override {};
	virtual void Init() override {};
	virtual void Reset() override {};
	virtual void Draw(Matrix4* vpMat_)override {};

	inline auto* Getter_Trans()
	{
		return &trans;
	}

	inline Status GetStatus()
	{
		return status;
	}

};

