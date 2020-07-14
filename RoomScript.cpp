#include "stdafx.h"
#include "RoomScript.h"
#include "Room.h"

void RoomScript::CollisionBegin(void* obj)
{
	Object* to = (Object*)obj;

	if (to->GetName() == "Probe")
	{
		Room* me = (Room*)_object;
		if (not me->GetIsMainRoom())
		{
			me->SetIsSubRoom(true);
		}
	}
}

void RoomScript::CollisionPreSolve(void* obj)
{



}

void RoomScript::CollisionEnd(void* obj)
{
}
