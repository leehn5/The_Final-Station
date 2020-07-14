#include "stdafx.h"
#include "EnemyAtkCollider.h"
#include "EnemyNormal.h"
#include "Player.h"
#include "PlayerDamage.h"

void EnemyAtkCollider::CollisionBegin(void* obj)
{
	Object* to = (Object*)obj;
	Enemy* me = (Enemy*)_object->GetParent();

	if (to->GetName().compare("Player") == 0)
	{
		Player* p = (Player*)obj;
		if (p->GetAbility()->IsDead() == false)
		{
			p->ChangeState(make_shared<PlayerDamage>(p));
			p->GetAbility()->DamageHP(me->GetAbility()->GetAttackPoint());
		}
	}
}

void EnemyAtkCollider::CollisionPreSolve(void* obj)
{
}

void EnemyAtkCollider::CollisionEnd(void* obj)
{
}
