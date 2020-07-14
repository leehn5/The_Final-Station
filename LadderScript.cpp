#include "stdafx.h"
#include "LadderScript.h"
#include "Tile.h"
#include "Player.h"

void LadderScript::CollisionBegin(void* obj)
{
	Object* to = (Object*)obj;
	Player* me = (Player*)_object->GetParent();

	if (to->GetTag().compare("Tile") == 0)
	{
		Tile* tile = (Tile*)obj;

		if (tile->GetAttribute() == TAttribute::LADDER)
		{
			me->SetIsLadderCollider(true);
			me->SetLadderPosition(to->GetTrans()->GetPos());
			me->SetLadderSize(to->GetTrans()->GetScale());
		}
	}
}

void LadderScript::CollisionPreSolve(void* obj)
{
}

void LadderScript::CollisionEnd(void* obj)
{
	Object* to = (Object*)obj;
	Player* me = (Player*)_object->GetParent();

	if (to->GetTag().compare("Tile") == 0)
	{
		Tile* tile = (Tile*)obj;

		if (tile->GetAttribute() == TAttribute::LADDER)
		{
			me->SetIsLadderCollider(false);
		}
	}
}
