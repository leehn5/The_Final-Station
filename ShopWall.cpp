#include "stdafx.h"
#include "ShopWall.h"
#include "Transform.h"


void ShopWall::Init(Vector2 pos, Vector2 scale)
{

	_tag = "ShopWall";
	_trans->SetPos(pos);
	_trans->SetScale(scale);

	_physics = AddComponent<PhysicsBody>();
	_physics->Init(BodyType::STATIC, 1.0f);
}
