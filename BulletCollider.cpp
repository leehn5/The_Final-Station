#include "stdafx.h"
#include "BulletCollider.h"
#include "BulletManager.h"
#include "EnemyNormal.h"
#include "Player.h"
#include "EnemyState.h"

void BulletCollider::CollisionBegin(void* obj)
{
	Object* to = (Object*)obj;
	Bullet* me = (Bullet*)_object;

	if (to->GetTag().compare("Enemy") == 0)
	{
		Player* p = (Player*)SCENEMANAGER->GetNowScene()->GetChildFromName("Player");
		Enemy* e;

		if (to->GetName() == "Head")
		{
			e = (Enemy*)to->GetParent();

			if (e->GetAbility()->IsDead() == false)
			{
				e->ChangeState(make_shared<EnemyDeadHeadshot>(e));
				e->GetAbility()->DamageHP(e->GetAbility()->GetMaxHP());
			}
		}
		else
		{
			e = (Enemy*)obj;

			if (e->GetAbility()->IsDead() == false)
			{
				e->ChangeState(make_shared<EnemyDamage>(e));
				e->GetPhysics()->ApplyForce(b2Vec2((int)p->GetCursorWhere(), 0) * 1000.f * TIMEMANAGER->getElapsedTime());
				e->GetAbility()->DamageHP(me->GetAttackPoint());
			}
		}

		me->SetIsFire(false);
	}
	if (to->GetTag().compare("Tile") == 0)
	{
		Tile* t = (Tile*)obj;
		if (t->GetAttribute() == TAttribute::WALL)
		{
			me->SetIsFire(false);
		}
		if (t->GetAttribute() == TAttribute::DOOR)
		{
			me->SetIsFire(false);
		}
	}
}

void BulletCollider::CollisionPreSolve(void* obj)
{
}

void BulletCollider::CollisionEnd(void* obj)
{
}
