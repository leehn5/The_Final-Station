#include "stdafx.h"
#include "PistolShell.h"

void PistolShell::Init()
{
	Object::Init();

	_speed = 1.0f;

	_sprite = AddComponent<Sprite>();
	_sprite->Init();
	_sprite->SetImgName("Pistol_Shell");
	_sprite->SetDepth(7);

	_physics = AddComponent<PhysicsBody>();
	_physics->Init(BodyType::DYNAMIC, 1.0f, 1.0f, 0.3f);

	b2Filter b;
	b.categoryBits = CATEGORY_PISTOL_SHELL;
	b.maskBits = MASK_PISTOL_SHELL;

	_physics->GetBody()->GetFixtureList()->SetFilterData(b);

	if(_dir == 1) 
		_physics->ApplyForce(b2Vec2(cosf(PI / 3), -sinf(PI / 3)) * _speed * TIMEMANAGER->getElapsedTime());
	else if(_dir == -1)
		_physics->ApplyForce(b2Vec2(cosf(PI2 / 3), -sinf(PI2 / 3)) * _speed * TIMEMANAGER->getElapsedTime());
}

void PistolShell::Release()
{
	Object::Release();
}

void PistolShell::Update()
{
	Object::Update();

	//_trans->SetRotateToRadian(_trans->GetRotateRadian() + 0.5f);
	//_physics->GetBody()->SetAngularVelocity(5.5f);
	//_physics->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(cosf(PI / 4), -sinf(PI / 4)), true);
	_sprite->SetPosition(_trans->GetPos());
}

void PistolShell::Render()
{
	Object::Render();
}
