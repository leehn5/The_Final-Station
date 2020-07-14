#include "stdafx.h"
#include "Train.h"

void Train::Init()
{
	Object::Init();

	_sprite = AddComponent<Sprite>();

	if (_mode == TrainMode::Show_In)
	{
		GRAPHICMANAGER->AddImage("Open_Train", L"Resource/Train/Open_Train.png");
		GRAPHICMANAGER->AddImage("Vent", L"Resource/Train/Vent.png");
		GRAPHICMANAGER->AddImage("t_wheels", L"Resource/Train/t_wheels.png", 8, 1);
		GRAPHICMANAGER->AddImage("stops", L"Resource/Train/stops.png", 6, 1);
		GRAPHICMANAGER->AddImage("stops_White", L"Resource/Train/stops_White.png");

		_sprite->SetImgName("Open_Train");

		_wheels = AddComponent<Sprite>();
		_wheels->Init(true, true);
		_wheels->SetImgName("t_wheels");
		_wheels->SetFPS(4.f);
		_wheels->SetPosition(_trans->GetPos() + Vector2(357, 84));

		_vant = Object::CreateObject<Object>(this);
		_vant->GetTrans()->SetPos(WINSIZEX / 2, 439);
		Sprite* s = _vant->AddComponent<Sprite>();
		s->SetImgName("Vent");
		s->SetDepth(2);

		_stops = Object::CreateObject<Object>(this);
		_stops->GetTrans()->SetPos(WINSIZEX / 2 + 330, 403);
		_stops->GetTrans()->SetScale(30, 60);
		_stops->SetName("stops");
		s = _stops->AddComponent<Sprite>();
		s->Init(true);
		s->SetImgName("stops");
		s->Stop();
		s->SetFPS(1.5f);
		s->SetPosition(_stops->GetTrans()->GetPos() + Vector2(0, 20.f));
		s->SetDepth(2);

		_food = Object::CreateObject<FoodObject>(this);
		_food->GetTrans()->SetPos(_trans->GetPos() + Vector2(186, -10));

		_medkit = Object::CreateObject<MedkitObject>(this);
		_medkit->GetTrans()->SetPos(_trans->GetPos() + Vector2(256, -10));

		_vent = Object::CreateObject<VentObject>(this);
		_vent->GetTrans()->SetPos(_trans->GetPos() + Vector2(52, -5));

		_isArrive = false;

		float gap = 0.f;

		for (int i = 0; i < 6; ++i)
		{
			_chairs[i] = Vector2(193 + (i * 53) + gap, 464);
			if (i % 2 == 1) gap += 49.f;
		}
	}
	else
	{
		GRAPHICMANAGER->AddImage("Train", L"Resource/Train/Train.png");
		_sprite->SetImgName("Train");
	}
}

void Train::Release()
{
	Object::Release();
}

void Train::Update()
{
	Object::Update();

	if (_mode == TrainMode::Show_In)
		_vant->GetTrans()->SetRotateToRadian(_vant->GetTrans()->GetRotateRadian() + 5.5f * TIMEMANAGER->getElapsedTime());
}

void Train::Render()
{
	Object::Render();

	if (_mode == TrainMode::Show_In)
	{
		if(_isArrive)
			GRAPHICMANAGER->Text(_stops->GetTrans()->GetPos() + Vector2(-100, -90), L"[YOU ARRIVED]", 20, 200, 20, ColorF::Azure, TextPivot::CENTER, L"Munro", true);
	}
}

void Train::ArriveAtTheStation()
{
	Sprite* s = _stops->GetComponent<Sprite>();
	if (s->GetCurrentFrameX() == 0) s->Start();
	if (s->GetCurrentFrameX() == s->GetMaxFrameX() - 1)
	{
		PhysicsBody* p = _stops->AddComponent<PhysicsBody>();
		p->Init(BodyType::STATIC, 1.f);
		p->SetSensor(true);

		_isArrive = true;
	}
}
