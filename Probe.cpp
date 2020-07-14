#include "stdafx.h"
#include "Probe.h"
#include "ProceduralTest.h"

void Probe::Init(Vector2 pos)
{
	Object::Init();
	_name = "Probe";
	_trans->SetPos(pos);
	_trans->SetScale(10, 10);
	_physics = AddComponent<PhysicsBody>();
	_physics->Init(BodyType::DYNAMIC, 1);
	_physics->GetBody()->SetFixedRotation(true);
	_physics->SetSensor(true);
	speed = 300;
}

void Probe::Update()
{
	Object::Update();

	SetTileAttribute();
	Move();

	if (!path.size()) _isActive = false;
}

void Probe::Release()
{
	SCENEMANAGER->GetNowScene()->GetWorld()->DestroyBody(_physics->GetBody());
	Object::Release();
}

void Probe::SetPath(list<Vector2> path)
{
	this->path.clear();
	this->path = path;
	int a;
}

void Probe::Move()
{
	if (path.size())
	{
		Vector2 normalize = Vector2(*path.begin() - _trans->GetPos()).Nomalized();
		_trans->SetPos(_trans->GetPos() + normalize * speed * TIMEMANAGER->getElapsedTime());
		_physics->SetBodyPosition();
		
		
		int a;

		if (Vector2::Distance(*path.begin(), _trans->GetPos()) < 20)
			path.pop_front();
	}

}

void Probe::SetTileAttribute()
{
	for (int i = 0; i < 9; i++)
	{
		Vector2 tileIdx((_trans->GetPos().x / TILE_WIDTH) + i % 3, (_trans->GetPos().y / TILE_HEIGHT) + i / 3);
		auto t = tiles[(int)tileIdx.x + (MAP_TILE_MAX_X + 1) * (int)tileIdx.y];
		t->SetAttribute(TAttribute::NONE);
		auto s = t->GetSprite();
		s->SetFillRect(true);
		s->SetRectColor(ColorF::Coral);
	}
}
