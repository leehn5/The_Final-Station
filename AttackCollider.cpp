#include "stdafx.h"
#include "AttackCollider.h"
#include "Player.h"
#include "EnemyNormal.h"
#include "EnemyState.h"

void AttackCollider::CollisionBegin(void* obj)
{
	Object* to = (Object*)obj;
	Player* me = (Player*)_object->GetParent();

	if (to->GetTag().compare("Enemy") == 0)
	{
		Enemy* e;

		if (to->GetName() == "Head")
		{
			e = (Enemy*)to->GetParent();

			if (e->GetAbility()->IsDead() == false)
			{
				CAMERA->ShakingSetting(0.3f, 1.5f);
				e->ChangeState(make_shared<EnemyDamage>(e));
				e->GetAbility()->DamageHP(me->GetAbility()->GetAttackPoint());
			}
		}
		else
		{
			e = (Enemy*)obj;

			if (e->GetAbility()->IsDead() == false)
			{
				CAMERA->ShakingSetting(0.3f, 1.5f);
				e->ChangeState(make_shared<EnemyDamage>(e));
				e->GetAbility()->DamageHP(me->GetAbility()->GetAttackPoint());
			}
		}
	}
}

void AttackCollider::CollisionPreSolve(void* obj)
{
}

void AttackCollider::CollisionEnd(void* obj)
{
}
