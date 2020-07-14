#include "stdafx.h"
#include "VentObject.h"

void VentObject::Init()
{
	InteractionObject::Init();

	_name = "panel_2";

	GRAPHICMANAGER->AddImage("panel_2", L"Resource/Train/UI/panel_2.png");
	GRAPHICMANAGER->AddImage("panel_2_White", L"Resource/Train/UI/panel_2_White.png");

	_trans->SetScale(GRAPHICMANAGER->FindImage("panel_2")->GetFrameWidth() - 20.f, GRAPHICMANAGER->FindImage("panel_2")->GetFrameHeight());
	_sprite->SetImgName("panel_2");

	_physics = AddComponent<PhysicsBody>();
	_physics->Init(BodyType::STATIC, 1.f);
	_physics->SetSensor(true);
}

void VentObject::Update()
{
}

void VentObject::Interaction()
{
}
