#include "stdafx.h"
#include "NPCCollider.h"
#include "NPC.h"
#include "Player.h"
#include "UI.h"
#include "NPC_Info.h"

void NPCCollider::CollisionBegin(void* obj)
{
	Object* to = (Object*)obj;
	NPC* me = (NPC*)_object;

	if (to->GetName().compare("Player") == 0)
	{
		Player* player = (Player*)obj;
		player->SetCollisionType(ColType::NPC);
		me->SetIsShowInfo(true);
		UI::GetNPCName(me->GetName());
	}
}

void NPCCollider::CollisionPreSolve(void* obj)
{
}

void NPCCollider::CollisionEnd(void* obj)
{
	Object* to = (Object*)obj;
	NPC* me = (NPC*)_object;

	if (to->GetName().compare("Player") == 0)
	{
		Player* player = (Player*)obj;
		player->SetCollisionType(ColType::None);
		me->SetIsShowInfo(false);
	}
}
