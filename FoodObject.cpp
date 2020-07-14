#include "stdafx.h"
#include "FoodObject.h"

void FoodObject::Init()
{
	InteractionObject::Init();

	_name = "food_box";

	GRAPHICMANAGER->AddImage("food_box", L"Resource/Train/UI/food_box.png");
	GRAPHICMANAGER->AddImage("food_box_White", L"Resource/Train/UI/food_box_White.png");

	_trans->SetScale(GRAPHICMANAGER->FindImage("food_box")->GetFrameWidth() - 26, GRAPHICMANAGER->FindImage("food_box")->GetFrameHeight() - 40);
	_sprite->SetImgName("food_box");
	_sprite->SetDepth(3);

	_physics = AddComponent<PhysicsBody>();
	_physics->Init(BodyType::STATIC, 1.f);
	_physics->SetSensor(true);
}

void FoodObject::Update()
{
	InteractionObject::Update();
}

void FoodObject::Interaction()
{
}
