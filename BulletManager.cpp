#include "stdafx.h"
#include "BulletManager.h"
#include "PistolShell.h"
#include "Player.h"
#include "BulletCollider.h"

void Bullet::Init()
{
	Object::Init();

	_tag = "Bullet";

	_trans->SetScale(10, 5);

	_sprite = AddComponent<Sprite>();
	_sprite->Init();
	//_sprite->SetShowRect(true);
	_sprite->SetDepth(7);

	_speed = 10.f;
	_atk = 10.f;
	_isFire = false;

	AddComponent<BulletCollider>();
}

void Bullet::Update()
{
	if (_isActive == false) return;

	Object::Update();

}

void Bullet::SetPhysicsBody()
{
	_physics = AddComponent<PhysicsBody>();
	_physics->Init(BodyType::DYNAMIC, 1.0f, 1.0f, 0.0f, true, true);
	_physics->GetBody()->SetFixedRotation(true);
	_physics->GetBody()->SetGravityScale(0.0f);
}


void BulletManager::Init()
{
	//_bulletMax = maxBullet;
	_player = (Player*)SCENEMANAGER->GetNowScene()->GetChildFromName("Player");

	/*for (int i = 0; i < _bulletMax; ++i)
	{
		Bullet* b = Object::CreateObject<Bullet>();
		b->Init();
		b->SetPhysicsBody();
		b->GetPhysics()->GetBody()->SetActive(false);
		b->SetIsActive(false);
		_vBullet.push_back(b);
	}*/
}

void BulletManager::Release()
{
	_vBullet.clear();
}

void BulletManager::Update()
{
	Move();

	for (Bullet* b : _vBullet)
	{
		if (b->GetIsActive() && b->GetIsFire() == false)
		{
			RemoveBullet(b);
		}
	}
}

void BulletManager::Render()
{
	for (Bullet* b : _vBullet)
	{
		if (b->GetIsActive() == false) continue;
		
		GRAPHICMANAGER->DrawLine(b->GetStartPosition() + Vector2(23 * _dir, 0), b->GetTrans()->GetPos(), ColorF(ColorF::Azure, 0.1f), 1.0f, true);
	}
}

void BulletManager::Fire(Vector2 pos, float speed, float angle, float atk, int dir)
{
	Bullet* b = Object::CreateObject<Bullet>();
	b->Init();
	b->SetPhysicsBody();

	b->GetTrans()->SetPos(pos);
	b->GetTrans()->SetRotateToRadian(-angle);
	b->SetSpeed(speed);
	b->SetAttackPoint(atk);
	b->SetIsFire(true);
	b->SetStartPosition(pos);
	b->GetPhysics()->SetBodyPosition();
	b->GetPhysics()->GetBody()->SetTransform(b2Vec2(b->GetPhysics()->GetBody()->GetPosition().x, b->GetPhysics()->GetBody()->GetPosition().y), b->GetTrans()->GetRotateRadian());
	b->GetPhysics()->ApplyForce(b2Vec2(cosf(angle), -sinf(angle)) * b->GetSpeed() * TIMEMANAGER->getElapsedTime());

	_vBullet.push_back(b);

	_dir = dir;

	_pistolShell = Object::CreateObject<PistolShell>();
	_pistolShell->GetTrans()->SetPos(pos + Vector2((int)_player->GetCursorWhere() * 10, 0.f));
	_pistolShell->SetDirection((int)_player->GetCursorWhere());
	_pistolShell->Init();
}

void BulletManager::Move()
{
	for (Bullet* b : _vBullet)
	{
		if (b->GetIsActive() == false) continue;
		
		if (TILE_WIDTH * 12 < Vector2::Distance(b->GetStartPosition(), b->GetTrans()->GetPos()))
		{
			b->SetIsRelese();
			_vBullet.remove(b);
			break;
		}
	}
}

void BulletManager::AddBullet(int value)
{
	for (int i = 0; i < value; ++i)
	{
		Bullet* b = Object::CreateObject<Bullet>();
		b->Init();
		b->SetPhysicsBody();
		b->GetPhysics()->GetBody()->SetActive(false);
		b->SetIsActive(false);
		_vBullet.push_back(b);
	}
}

void BulletManager::Reload(int& magazine, int& fireCount)
{
	int c = magazine - fireCount;
	if (c < 0) return;

	magazine -= fireCount;
	AddBullet(c);
}

void BulletManager::RemoveBullet(Bullet* b)
{
	b->SetIsRelese();
	_vBullet.remove(b);
}
