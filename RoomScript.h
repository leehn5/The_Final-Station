#pragma once
#include "Collider.h"
class RoomScript : public Collider
{
public:

	virtual void CollisionBegin(void* obj);
	//�浹��
	virtual void CollisionPreSolve(void* obj);
	//�浹 ���� ����
	virtual void CollisionEnd(void* obj);
};

