#include "GameObjectBehavior.h"
#include "../GameObjectManager/GameObjectManager.h"


//新しくゲームオブジェクトを作ったらテーブルに追加してほしい
void GameObjectBehavior::SetIdentity(Tag tag_)
{
	int no = identity.number;
	std::string id = "[" + std::to_string(no) + "] : ";

	if (tag_ == Tag::kNone)
	{
		identity =  { id + "NoName" ,tag_,no,0,0 };
		return;
	}

	Identity identitiesTable[Tag::kCount]
	{
		Identity{"Enemy" ,tag_,no,				0x0000000f,0x0ffff000},
		Identity{"EnemyFactory" ,tag_,no,		0x000000f0,0x000f0000},
		Identity{"EnemyTower" ,tag_,no,			0x00000f00,0x000f0000},
		Identity{"Player" ,tag_,no,				0x0000f000,0x0f00000f},
		Identity{"PlayerBullet" ,tag_,no,		0x000f0000,0x0f000fff},
		Identity{"PlayerTower" ,tag_,no,		0x00f00000,0x0000000f},
		Identity{"PlayerAlly" ,tag_,no,			0x0f000000,0x000ff00f},
		Identity{"InGameController" ,tag_,no,	0x00000000,0x00000000},
		Identity{"UIManager" ,tag_,no,			0x00000000,0x00000000},
	};

	identitiesTable[(int)tag_].name = id + identitiesTable[(int)tag_].name;

	identity = identitiesTable[(int)tag_];
}

std::string GameObjectBehavior::Getter_Name()
{
	return identity.name;
}

void GameObjectBehavior::SwitchCollisionActivation(bool bool_)
{
	collision.collisionActivate = bool_;
}

bool GameObjectBehavior::IsCollisionActivated()
{
	return collision.collisionActivate;
}

GameObjectBehavior::Identity::Identity()
{
	name = "noName";
	tag = kNone;
	number = 0;
	collisionAttribute = 0;
	collisionMask = 0;
}

bool GameObjectBehavior::Identity::IsCollisionMaskMatched(Identity* other_)
{	
	return !(collisionAttribute & other_->collisionMask);
}

GameObjectBehavior::Identity::Identity(std::string name_, Tag tag_, int number_, 
	uint32_t att_, uint32_t mask_)
{
	name = name_;
	tag = tag_;
	number = number_;
	collisionAttribute = att_;
	collisionMask = mask_;
}

bool GameObjectBehavior::IsCollisionMaskMatched(Identity* other_)
{
	return identity.IsCollisionMaskMatched(other_);
}

GameObjectBehavior::Identity* GameObjectBehavior::Getter_Identity()
{
	return &identity;
}

Rect* GameObjectBehavior::Getter_Rect()
{
	return collision.rect.get();;
}

Circle* GameObjectBehavior::Getter_Circle()
{
	return collision.circle.get();
}

void GameObjectBehavior::SetGameObjectManager(GameObjectManager* gameObjectManager_)
{
	gameObjectManager = gameObjectManager_;
}


bool GameObjectBehavior::HasRectCollider()
{
	if (collision.rect) return true;

	return false;
}

bool GameObjectBehavior::HasCircleCollider()
{
	if (collision.circle) return true;

	return false;
}

bool GameObjectBehavior::HasCollider()
{
	return HasRectCollider() + HasCircleCollider();
}

void GameObjectBehavior::SetRectCollision(float width_, float height_ , Vector3 centerPos_)
{
	if (!collision.rect) collision.rect.reset(new Rect);

	collision.rect->SetShape(width_, height_, centerPos_);
}

void GameObjectBehavior::SetCollisionBack(Tag tag_, std::function<void()> func_)
{
	collision.collisionBackActivationMap[tag_] = func_;
}


void GameObjectBehavior::SetCircleCollision(float radius_)
{
	if (!collision.circle) collision.circle.reset(new Circle);

	collision.circle->SetShape(radius_);
}

void GameObjectBehavior::ActivateOnTriggerEnter(GameObjectBehavior::Tag tag_)
{
#ifdef _DEBUG
	forDebug.colorForCollision = { 200,50,50,255 };
#endif // _DEBUG

	if (!collision.collisionBackActivationMap[tag_]) return;

	collision.collisionBackActivationMap[tag_]();
}

void GameObjectBehavior::SetNumber(int number_)
{
	identity.number = number_;
}

void GameObjectBehavior::SetStatus(Status dst_)
{
	status = dst_;
}

GameObject::GameObject()
{
	status = Status::kActive;
}

void GameObjectBehavior::SetCollidedObjPtr(GameObjectBehavior* obj_)
{
	collision.colObj = obj_;
}

GameObjectBehavior* GameObjectBehavior::Getter_ColObj()
{
	return collision.colObj;
}