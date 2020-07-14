#include "stdafx.h"
#include "Room.h"
#include "RoomScript.h"
void Room::Init()
{
	Object::Init();
	_tag = "Room";
	_trans->SetPos(RND->getFromIntTo(MAP_MAX_WIDTH / 2 - 700, MAP_MAX_WIDTH/2 + 700) , RND->getFromIntTo(MAP_MAX_HEIGHT / 2-100, MAP_MAX_HEIGHT / 2 + 100));
	
	_trans->SetScale(RND->getFromFloatTo(100.f, 300.f), RND->getFromFloatTo(100.f, 300));

	_physics = AddComponent<PhysicsBody>();
	_physics->Init(BodyType::DYNAMIC, 1.f, 1.f, 0.f, false, true);
	_physics->GetBody()->SetFixedRotation(true);
	AddComponent<RoomScript>();
	//	_physics->SetBodyActive(false);
}

void Room::Update()
{
	return;
}
