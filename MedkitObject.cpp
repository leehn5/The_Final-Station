#include "stdafx.h"
#include "MedkitObject.h"

void MedkitObject::Init()
{
	InteractionObject::Init();

	_name = "medkit_box";

	GRAPHICMANAGER->AddImage("medkit_box", L"Resource/Train/UI/medkit_box.png");
	GRAPHICMANAGER->AddImage("medkit_box_White", L"Resource/Train/UI/medkit_box_White.png");

	_trans->SetScale(GRAPHICMANAGER->FindImage("medkit_box")->GetFrameWidth() - 26, GRAPHICMANAGER->FindImage("medkit_box")->GetFrameHeight() - 40);
	_sprite->SetImgName("medkit_box");
	_sprite->SetDepth(3);

	_physics = AddComponent<PhysicsBody>();
	_physics->Init(BodyType::STATIC, 1.f);
	_physics->SetSensor(true);
}

void MedkitObject::Udate()
{
	InteractionObject::Update();
}

void MedkitObject::Interaction()
{
}
