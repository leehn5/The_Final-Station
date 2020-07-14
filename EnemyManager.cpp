#include "stdafx.h"
#include "EnemyManager.h"
#include "EnemyNormal.h"

void EnemyManager::Init()
{
	SOUNDMANAGER->addSound("Hit Swoosh_heavy", "Resource/Sound/Hit Swoosh_heavy.mp3", false, false);
}

void EnemyManager::Release()
{
	for (Enemy* e : _vEnemy)
	{
		SCENEMANAGER->GetNowScene()->GetWorld()->DestroyBody(e->GetComponent<PhysicsBody>()->GetBody());

		e->Release();
	}
	_vEnemy.clear();
}

void EnemyManager::Update()
{
	for (Enemy* e : _vEnemy)
	{
		if (e->GetIsActive() == false)
		{
			e->GetHead()->SetIsRelese();
			//e->SetIsRelese();
			_vEnemy.remove(e);
			break;
		}
	}
}

void EnemyManager::Render()
{
}

void EnemyManager::SetEnemy(Vector2 pos)
{
	Enemy* e = Object::CreateObject<EnemyNormal>();
	e->Init();
	e->GetTrans()->SetPos(pos);
	e->GetPhysics()->SetBodyPosition();
	e->GetHead()->GetTrans()->SetPos(pos + Vector2(0, -28));
	e->GetHead()->GetComponent<PhysicsBody>()->SetBodyPosition();
	_vEnemy.push_back(e);
}
