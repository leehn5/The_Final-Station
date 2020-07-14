#include "stdafx.h"
#include "HandAttack.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "AttackCollider.h"
#include "PlayerLadder.h"

void HandAttack::Enter()
{
	_state = "Hand_Attack";

	_obj->GetSprite()->SetImgName("Hero_Pistol_Superhit");
	_obj->GetSprite()->SetFPS(2.0f);

	_atkObj = Object::CreateObject<Object>(_obj);
	_atkObj->GetTrans()->SetScale(20, 20);
	_atkObj->SetName("atkObj");
	_atkObj->AddComponent<AttackCollider>();

	if(_obj->GetBody()->GetComponent<Sprite>()->GetFlipX())
		_atkObj->GetTrans()->SetPos(_obj->GetTrans()->GetPos() + Vector2(-20.f, 0.f));
	else
		_atkObj->GetTrans()->SetPos(_obj->GetTrans()->GetPos() + Vector2(20.f, 0.f));

	PhysicsBody* p = _atkObj->AddComponent<PhysicsBody>();
	p->Init(BodyType::DYNAMIC, 5.f, 5.f);
	p->GetBody()->SetFixedRotation(true);
	p->SetSensor(true);
	p->GetBody()->SetGravityScale(0.f);

}

void HandAttack::Update()
{
	if(_obj->GetPrevState() != "Ladder") _obj->Move();

	if (_obj->GetSprite()->IsFrameEnd())
	{
		if(_obj->GetPrevState() == "Ladder")
			_obj->ChangeState(make_shared<PlayerLadder>(_obj));
		else
			_obj->ChangeState(make_shared<PlayerIdle>(_obj));
	}
}

void HandAttack::Exit()
{
	_obj->GetChildFromName("atkObj")->SetIsRelese();
}
