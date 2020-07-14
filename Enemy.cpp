#include "stdafx.h"
#include "Enemy.h"
#include "EnemyState.h"

void Enemy::Init()
{
	Object::Init();

	_tag = "Enemy";

	_sprite = AddComponent<Sprite>();

	_physics = AddComponent<PhysicsBody>();

	_head = Object::CreateObject<Object>(this);
	_head->SetName("Head");
	_head->SetTag("Enemy");
	_head->AddComponent<Sprite>();
	_head->AddComponent<PhysicsBody>();
}

void Enemy::Release()
{
	Object::Release();
}

void Enemy::Update()
{
	Object::Update();

	if (_dir == E_Dir::Left) _sprite->SetFlipX(true);
	else if (_dir == E_Dir::Right) _sprite->SetFlipX(false);
}

void Enemy::Render()
{
	Object::Render();
}

void Enemy::ChangeState(shared_ptr<EnemyState> state)
{
	_state->Exit();
	_state.swap(state);
	_state->Enter();
}
