#include "stdafx.h"
#include "EnemyState.h"
#include "Player.h"
#include "EnemyNormal.h"
#include "EnemyAtkCollider.h"

// Idle
void EnemyIdle::Enter()
{
	_state = "Idle";
	_enemy->GetSprite()->SetImgName(_enemy->GetName() + "_Idle");
	_enemy->GetSprite()->SetIsLoop(true);

	_player = (Player*)SCENEMANAGER->GetNowScene()->GetChildFromName("Player");

	_timer = 0.0f;
}

void EnemyIdle::Update()
{
	//_distance = fabs(Vector2::Distance(_enemy->GetTrans()->GetPos(), _player->GetTrans()->GetPos()));
	_distance = fabs(_enemy->GetTrans()->GetPos().x - _player->GetTrans()->GetPos().x);

	float height = fabs(_enemy->GetTrans()->GetPos().y - _player->GetTrans()->GetPos().y);

	if (!_player->GetAbility()->IsDead() && (_distance <= TILE_WIDTH * 10 && height <= TILE_HEIGHT * 4))
	{
		_timer += 1.f * TIMEMANAGER->getElapsedTime();
		if (_timer >= 0.5f)
		{
			_enemy->ChangeState(make_shared<EnemyMove>(_enemy));
			return;
		}
	}
}

void EnemyIdle::Exit()
{
}

// Move
void EnemyMove::Enter()
{
	_state = "Move";
	
	_enemy->GetSprite()->SetImgName(_enemy->GetName() + "_Walk");
	_enemy->GetSprite()->SetIsLoop(true);

	_player = (Player*)SCENEMANAGER->GetNowScene()->GetChildFromName("Player");
}

void EnemyMove::Update()
{
	_v2Dis = (_enemy->GetTrans()->GetPos() - _player->GetTrans()->GetPos());

	if (_v2Dis.x <= -1.f) _enemy->SetDirection(E_Dir::Right);
	else if (_v2Dis.x >= 1.f) _enemy->SetDirection(E_Dir::Left);
	else _enemy->SetDirection(E_Dir::None);

	if (fabs(_v2Dis.x) <= TILE_WIDTH && fabs(_v2Dis.y) <= TILE_HEIGHT)
	{
		_enemy->ChangeState(make_shared<EnemyAttack>(_enemy));
		return;
	}

	if (fabs(_v2Dis.x) > TILE_WIDTH * 10 || fabs(_v2Dis.y) > TILE_HEIGHT * 4)
	{
		_enemy->ChangeState(make_shared<EnemyIdle>(_enemy));
		return;
	}

	_enemy->GetPhysics()->ApplyForce(Vector2::b2Down);
	_enemy->GetTrans()->SetPos(_enemy->GetTrans()->GetPos() + Vector2(_enemy->GetDirection(), 0.0f) *
	_enemy->GetAbility()->GetSpeed() * TIMEMANAGER->getElapsedTime());
	
	_enemy->GetPhysics()->SetBodyPosition();

	_enemy->GetHead()->GetTrans()->SetPos(_enemy->GetTrans()->GetPos() + Vector2(0, -28));
	_enemy->GetHead()->GetComponent<PhysicsBody>()->SetBodyPosition();
}

void EnemyMove::Exit()
{
}

// Attack
void EnemyAttack::Enter()
{
	_state = "Attack";

	_enemy->GetSprite()->SetImgName(_enemy->GetName() + "_Attack");
	_enemy->GetSprite()->SetIsLoop(false);

	_timer = 0.0f;

	_atkArea = Object::CreateObject<Object>(_enemy);
	_atkArea->GetTrans()->SetScale(30, 30);
	_atkArea->SetName("atkArea");
	_atkArea->AddComponent<EnemyAtkCollider>();

	if (_enemy->GetComponent<Sprite>()->GetFlipX())
		_atkArea->GetTrans()->SetPos(_enemy->GetTrans()->GetPos() + Vector2(-23.f, 0.f));
	else
		_atkArea->GetTrans()->SetPos(_enemy->GetTrans()->GetPos() + Vector2(23.f, 0.f));

	PhysicsBody* p = _atkArea->AddComponent<PhysicsBody>();
	p->Init(BodyType::DYNAMIC, 5.f, 5.f);
	p->GetBody()->SetFixedRotation(true);
	p->SetSensor(true);
	p->GetBody()->SetGravityScale(0.f);
	p->SetBodyActive(false);

	SOUNDMANAGER->play("Hit Swoosh_heavy");
}

void EnemyAttack::Update()
{
	if (_enemy->GetSprite()->GetCurrentFrameX() >= 4)
	{
		_atkArea->GetComponent<PhysicsBody>()->SetBodyActive(true);
	}
	if (_enemy->GetSprite()->IsFrameEnd())
	{
		_timer += 1.0f * TIMEMANAGER->getElapsedTime();
		if (_timer > 0.25f)
		{
			_enemy->ChangeState(make_shared<EnemyIdle>(_enemy));
			return;
		}
	}
}

void EnemyAttack::Exit()
{
	_enemy->GetChildFromName("atkArea")->SetIsRelese();
}

// Damege
void EnemyDamage::Enter()
{
	_state = "Damege";
	_enemy->GetSprite()->SetImgName(_enemy->GetName() + "_Damage");
	_enemy->GetSprite()->SetIsLoop(false);

	_timer = 0.0f;
}

void EnemyDamage::Update()
{
	if (_enemy->GetSprite()->IsFrameEnd())
	{
		_timer += 1.0f * TIMEMANAGER->getElapsedTime();
		if (_timer > 0.25f)
		{
			if (_enemy->GetAbility()->IsDead())
			{
				_enemy->ChangeState(make_shared<EnemyDead>(_enemy));
				return;
			}

			_enemy->ChangeState(make_shared<EnemyIdle>(_enemy));
			return;
		}
	}

	_enemy->GetHead()->GetTrans()->SetPos(_enemy->GetTrans()->GetPos() + Vector2(0, -28));
	_enemy->GetHead()->GetComponent<PhysicsBody>()->SetBodyPosition();
}

void EnemyDamage::Exit()
{
}

// Dead
void EnemyDead::Enter()
{
	_state = "Dead";

	_enemy->GetSprite()->SetImgName(_enemy->GetName() + "_Dead");
	_enemy->GetSprite()->SetIsLoop(false);

	_enemy->GetPhysics()->SetBodyActive(false);
	_enemy->GetHead()->GetComponent<PhysicsBody>()->SetBodyActive(false);

	_timer = 0.0f;
}

void EnemyDead::Update()
{
	if (_enemy->GetSprite()->IsFrameEnd())
	{
		_timer += 1.0f * TIMEMANAGER->getElapsedTime();
		if (_timer > 0.45f)
		{
			_enemy->GetHead()->SetIsActive(false);
			_enemy->SetIsActive(false);
		}
	}
}

void EnemyDead::Exit()
{
}

void EnemyDeadHeadshot::Enter()
{
	_state = "Dead";

	_enemy->GetSprite()->SetImgName(_enemy->GetName() + "_Dead_Headshot");
	_enemy->GetSprite()->SetIsLoop(false);

	_enemy->GetPhysics()->SetBodyActive(false);
	_enemy->GetHead()->GetComponent<PhysicsBody>()->SetBodyActive(false);

	_timer = 0.0f;
}

void EnemyDeadHeadshot::Update()
{
	if (_enemy->GetSprite()->IsFrameEnd())
	{
		_timer += 1.0f * TIMEMANAGER->getElapsedTime();
		if (_timer > 0.45f)
		{
			_enemy->GetHead()->SetIsActive(false);
			_enemy->SetIsActive(false);
		}
	}
}

void EnemyDeadHeadshot::Exit()
{
}
