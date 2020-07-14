#include "stdafx.h"
#include "Player.h"
#include "PlayerState.h"
#include "PlayerIdle.h"
#include "PlayerCollider.h"
#include "LadderScript.h"
#include "BulletManager.h"
#include "Item.h"

void Player::Init()
{
	Object::Init();
	
	_name = "Player";

#pragma region AddImage
	GRAPHICMANAGER->AddImage("Hero_Dead", L"Resource/Player/Hero_Dead.png", 10, 1);
	GRAPHICMANAGER->AddImage("Hero_Idle", L"Resource/Player/Hero_Idle.png", 5, 1);
	GRAPHICMANAGER->AddImage("Hero_Idle_d", L"Resource/Player/Hero_Idle_d.png", 5, 1);
	GRAPHICMANAGER->AddImage("Hero_Idle_u", L"Resource/Player/Hero_Idle_u.png", 5, 1);
	GRAPHICMANAGER->AddImage("Legs_Idle", L"Resource/Player/Legs_Idle.png", 5, 1);
	GRAPHICMANAGER->AddImage("Legs_Run", L"Resource/Player/Legs_Run.png", 8, 1);
	GRAPHICMANAGER->AddImage("Legs_Run_Back", L"Resource/Player/Legs_Run_Back.png", 5, 1);
	GRAPHICMANAGER->AddImage("Hero_pistol_arm", L"Resource/Player/Hero_pistol_arm2.png", 5, 1);
	GRAPHICMANAGER->AddImage("Hero_Pistol_Superhit", L"Resource/Player/Hero_Pistol_Superhit.png", 5, 1);
	GRAPHICMANAGER->AddImage("Hero_Damage", L"Resource/Player/Hero_Damage.png", 3, 1);
	GRAPHICMANAGER->AddImage("Hero_Reload_Pistol", L"Resource/Player/Hero_Reload_Pistol.png", 16, 1);
	GRAPHICMANAGER->AddImage("train_idle", L"Resource/Player/train_hero_idle.png", 5, 1);
	GRAPHICMANAGER->AddImage("train_walk", L"Resource/Player/train_hero walk.png", 8, 1);
	GRAPHICMANAGER->AddImage("train_idle_holdItem", L"Resource/Player/train_holdItem_idle.png", 5, 1);
	GRAPHICMANAGER->AddImage("train_walk_holdItem", L"Resource/Player/train_holdItem_walk.png", 8, 1);
	GRAPHICMANAGER->AddImage("Pistol_Attack", L"Resource/Player/Pistol_Attack2.png", 5, 1);
	GRAPHICMANAGER->AddImage("Pistol_Shell", L"Resource/Player/Pistol_Shell.png");
	GRAPHICMANAGER->AddImage("None", L"Resource/Terrain/None/None.png");
#pragma endregion

	SOUNDMANAGER->addSound("Pistol Shoot", "Resource/Sound/Pistol Shoot.mp3", false, false);
	SOUNDMANAGER->addSound("Pistol Reload", "Resource/Sound/Pistol Reload.mp3", false, false);
	SOUNDMANAGER->addSound("Clip Empty", "Resource/Sound/Clip Empty.mp3", false, false);
	SOUNDMANAGER->addSound("Hit Swoosh", "Resource/Sound/Hit Swoosh.mp3", false, false);

	_sprite = AddComponent<Sprite>();
	_sprite->Init(true);
	_sprite->SetDepth(7);

	if (SCENEMANAGER->GetNowScene()->GetName() == "Train") _mode = Mode::Non_Combat;
	else _mode = Mode::Combat;

	if (_mode == Mode::Combat)
	{
		_sprite->SetImgName("None");
		_sprite->SetIsLoop(false);

		_trans->SetScale(20, 42);

		_legs = Object::CreateObject<Object>(this);
		_legs->GetTrans()->SetPos(_trans->GetPos() + Vector2(0, 5.0f));

		Sprite* s = _legs->AddComponent<Sprite>();
		s->Init(true, true);
		s->SetImgName("Legs_Idle");
		_legs->GetTrans()->SetScale(s->GetFrameWidth(), s->GetFrameHeight());


		_body = Object::CreateObject<Object>(this);
		_body->GetTrans()->SetPos(_trans->GetPos() + Vector2(0, 2.0f));

		s = _body->AddComponent<Sprite>();
		s->Init(true, true);
		s->SetImgName("Hero_Idle_u");
		_body->GetTrans()->SetScale(s->GetFrameWidth(), s->GetFrameHeight());


		_arms = Object::CreateObject<Object>(this);
		_arms->GetTrans()->SetPos(_trans->GetPos());

		s = _arms->AddComponent<Sprite>();
		s->Init(true, true);
		s->SetImgName("Hero_pistol_arm");
		_arms->GetTrans()->SetScale(s->GetFrameWidth(), s->GetFrameHeight());

		_cw = CursorWhere::None;
		_atkType = AtkType::Pistol;
		_colType = ColType::None;

		_physics = AddComponent<PhysicsBody>();
		_physics->Init(BodyType::DYNAMIC, 5.0f, 3.0f);
		_physics->GetBody()->SetFixedRotation(true);

		b2Filter b;
		b.categoryBits = CATEGORY_PLAYER;
		b.maskBits = MASK_PLAYER;
		_physics->GetBody()->GetFixtureList()->SetFilterData(b);


		_judgingFloor = Object::CreateObject<Object>(this);
		_judgingFloor->GetTrans()->SetScale(1, 1);
		_judgingFloor->GetTrans()->SetPos(_trans->GetPosToPivot(TF_PIVOT::BOTTOM) + Vector2::up * _judgingFloor->GetTrans()->GetScale().y / 2 - Vector2(0, 5));
		_judgingFloor->AddComponent<LadderScript>();

		_floorPhysics = _judgingFloor->AddComponent<PhysicsBody>();
		_floorPhysics->Init(BodyType::DYNAMIC, 1.0f, 1.0f);
		_floorPhysics->GetBody()->SetFixedRotation(true);
		_floorPhysics->SetSensor(true);
	}
	else
	{
		_sprite->SetImgName("train_idle");
		_sprite->SetIsLoop(true);

		_colType = ColType::None;

		_physics = AddComponent<PhysicsBody>();
		_physics->Init(BodyType::DYNAMIC, 5.0f, 3.0f);
		_physics->GetBody()->SetFixedRotation(true);

		b2Filter b;
		b.categoryBits = CATEGORY_PLAYER;
		b.maskBits = MASK_PLAYER;
		_physics->GetBody()->GetFixtureList()->SetFilterData(b);
	}

	_state = make_shared<PlayerIdle>(this);
	_state->Enter();

	_isLadderCol = false;
	_isHoldItem = false;

	_ability = make_shared<Ability>(100, 100, 10, SPEED);

	//_mInven.insert(make_pair("GOLD KEY", new Item("GOLD KEY", ItemType::Key, 0, 1)));

	_bulletMgr = new BulletManager;
	_bulletMgr->Init();

	Parse();

	_isMove = true;

	AddComponent<PlayerCollider>();
}

void Player::Release()
{
	ofstream outFile;
	outFile.open("PlayerInfo.json", ios::binary);

	json j;

	_mInven.find("PISTOL AMMO")->second->count = _reminingAmmo;

	map<string, Item*>::iterator it = _mInven.begin();
	for (; it != _mInven.end(); ++it)
	{
		j["Inven"][it->first]["name"] = it->second->name;
		j["Inven"][it->first]["type"] = it->second->type;
		j["Inven"][it->first]["count"] = it->second->count;
		j["Inven"][it->first]["price"] = it->second->price;
	}

	j["ammo"] = _ammo;
	j["magazine"] = _magazine;
	j["isBattery"] = _isBattery;
	j["money"] = _money;

	j["prevScene"] = SCENEMANAGER->GetNowScene()->GetName();

	outFile << std::setw(4) << j << endl;
	
	outFile.close();

	_bulletMgr->Release();

	Object::Release();
}

void Player::Update()
{
	Object::Update();

	if (_mode == Mode::Combat)
	{
		_judgingFloor->GetTrans()->SetPos(_trans->GetPosToPivot(TF_PIVOT::BOTTOM) + Vector2::up * _judgingFloor->GetTrans()->GetScale().y / 2 - Vector2(0, 5));
		_judgingFloor->GetComponent<PhysicsBody>()->SetBodyPosition();

		_state->Update();

		_trans->SetPos(_physics->GetBodyPosition());

		if (KEYMANAGER->isStayKeyDown('V') && _ability->GetCurrentHP() > 0.0f) _ability->setHP(_ability->GetCurrentHP() - 5.0f);
		if (KEYMANAGER->isStayKeyDown('B') && _ability->GetCurrentHP() < _ability->GetMaxHP()) _ability->setHP(_ability->GetCurrentHP() + 5.0f);

		if (_state->GetState().compare("Dead") == 0 || _state->GetState().compare("Hand_Attack") == 0)
		{
			_body->SetIsActive(false);
			_arms->SetIsActive(false);
			_legs->SetIsActive(false);

			if (_sprite->GetFlipX()) _sprite->SetPosition(_trans->GetPos() + Vector2(-6.0f, 1.0f));
			else _sprite->SetPosition(_trans->GetPos() + Vector2(6.0f, 1.0f));
		}
		else
		{
			if (_body->GetIsActive() == false)
			{
				_body->SetIsActive(true);
				_legs->SetIsActive(true);
				_arms->SetIsActive(true);

				_sprite->SetImgName("None");
			}

			if (_state.get()->GetState().compare("Idle") == 0 || _state.get()->GetState().compare("Move") == 0 ||
				_state.get()->GetState().compare("Ladder") == 0)
			{
				_body->GetTrans()->SetPos(_trans->GetPos() + Vector2(0, -2.0f));
				DirectionSprite();
			}
			_body->GetComponent<Sprite>()->SetPosition(_body->GetTrans()->GetPos());

			_legs->GetTrans()->SetPos(_trans->GetPos() + Vector2(0, 1.0f));
			_legs->GetComponent<Sprite>()->SetPosition(_legs->GetTrans()->GetPos());

			_arms->GetTrans()->SetPos(_trans->GetPos() + Vector2(8.0f * (int)_cw, -2.5f));
			_arms->GetComponent<Sprite>()->SetPosition(_arms->GetTrans()->GetPos());
			_arms->GetComponent<Sprite>()->SetAngleAnchor(Vector2(-13.f * (int)_cw, 0.0f));
		}
	}
	else
	{
		_state->Update();
		_sprite->SetPosition(_trans->GetPos());
		_trans->SetPos(_physics->GetBodyPosition());
	}

	if (KEYMANAGER->isOnceKeyDown('G')) _mInven.find("GOLD KEY")->second->count -= 1;
	if (KEYMANAGER->isOnceKeyDown('H')) _mInven.find("GOLD KEY")->second->count += 1;

	_bulletMgr->Update();
}

void Player::Render()
{
	Object::Render();

	//char buffer[128];
	//sprintf_s(buffer, "%s", _state->GetState().c_str());
	//GRAPHICMANAGER->Text(Vector2(WINSIZEX / 2, 200), buffer, 20, 200, 20, ColorF::Azure);

	//sprintf_s(buffer, "Money : %d", _money);
	//GRAPHICMANAGER->Text(Vector2(WINSIZEX / 2, 200), buffer, 20, 200, 20, ColorF::Azure);

	//sprintf_s(buffer, "%d", _isSlope);
	//GRAPHICMANAGER->Text(Vector2(WINSIZEX / 2, 220), buffer, 20, 200, 20, ColorF::Azure);

	//sprintf_s(buffer, "index : %d", ((int)MOUSEPOINTER->GetMouseWorldPosition().x / TILE_WIDTH) + TILE_NUM_X * ((int)MOUSEPOINTER->GetMouseWorldPosition().y / TILE_HEIGHT));
	//GRAPHICMANAGER->Text(Vector2(WINSIZEX / 2, 180), buffer, 20, 200, 20, ColorF::Azure);

	//sprintf_s(buffer, "%d", _colType);
	//GRAPHICMANAGER->Text(Vector2(WINSIZEX / 2, 220), buffer, 20, 200, 20, ColorF::Azure);

	_bulletMgr->Render();
}

void Player::ChangeState(shared_ptr<PlayerState> state)
{
	_state->Exit();
	_state.swap(state);
	_state->Enter();
}

void Player::Reload()
{
	if ((_magazine - _ammo) > _reminingAmmo)
	{
		_ammo = _reminingAmmo + _ammo;
		_reminingAmmo = 0;
	}
	else
	{
		_reminingAmmo = _reminingAmmo - (_magazine - _ammo);
		_ammo = (_magazine - _ammo) + _ammo;
	}
}

void Player::DirectionSprite()
{
	if (MOUSEPOINTER->GetMouseWorldPosition().x < _trans->GetPos().x)
	{
		_cw = CursorWhere::Left;

		_armsAngle = -Vector2::GetAngle(MOUSEPOINTER->GetMouseWorldPosition(), _trans->GetPos());
		_body->GetComponent<Sprite>()->SetFlipX(true);
		_legs->GetComponent<Sprite>()->SetFlipX(true);
		_arms->GetComponent<Sprite>()->SetFlipX(true);

		_sprite->SetFlipX(true);

		if (_armsAngle < -0.6f)
		{
			_arms->GetTrans()->SetRotateToRadian(-0.6f);
			_armsAngle = -0.6f;
		}
		else if (_armsAngle > 0.5f)
		{
			_arms->GetTrans()->SetRotateToRadian(0.5f);
			_armsAngle = 0.5f;
		}

		if (_armsAngle >= -0.6f and _armsAngle <= 0.5f)
		{
			_arms->GetTrans()->SetRotateToRadian(_armsAngle);

			if (_armsAngle < -0.25f)
			{
				if (_body->GetComponent<Sprite>()->GetImgKey().compare("Hero_Idle_d") != 0)
				{
					_body->GetComponent<Sprite>()->SetImgName("Hero_Idle_d");
					_arms->GetComponent<Sprite>()->Start();
				}
			}
			else if (_armsAngle < 0.25f)
			{
				if (_body->GetComponent<Sprite>()->GetImgKey().compare("Hero_Idle") != 0)
				{
					_body->GetComponent<Sprite>()->SetImgName("Hero_Idle");
					_arms->GetComponent<Sprite>()->Start();
				}
			}
			else if (_armsAngle < 0.6f)
			{
				if (_body->GetComponent<Sprite>()->GetImgKey().compare("Hero_Idle_u") != 0)
				{
					_body->GetComponent<Sprite>()->SetImgName("Hero_Idle_u");
					_arms->GetComponent<Sprite>()->Start();
				}
			}
		}
	}
	else
	{
		_cw = CursorWhere::Right;

		_armsAngle = -Vector2::GetAngle(_trans->GetPos(), MOUSEPOINTER->GetMouseWorldPosition());
		_body->GetComponent<Sprite>()->SetFlipX(false);
		_legs->GetComponent<Sprite>()->SetFlipX(false);
		_arms->GetComponent<Sprite>()->SetFlipX(false);

		_sprite->SetFlipX(false);

		if (_armsAngle < -0.5f)
		{
			_arms->GetTrans()->SetRotateToRadian(-0.5f);
			_armsAngle = -0.5f;
		}
		else if (_armsAngle > 0.6f)
		{
			_arms->GetTrans()->SetRotateToRadian(0.6f);
			_armsAngle = 0.6f;
		}

		if (_armsAngle >= -0.5f and _armsAngle <= 0.6f)
		{
			_arms->GetTrans()->SetRotateToRadian(_armsAngle);

			if (_armsAngle < -0.25f)
			{
				if (_body->GetComponent<Sprite>()->GetImgKey().compare("Hero_Idle_u") != 0)
				{
					_body->GetComponent<Sprite>()->SetImgName("Hero_Idle_u");
					_arms->GetComponent<Sprite>()->Start();
				}
			}
			else if (_armsAngle < 0.25f)
			{
				if (_body->GetComponent<Sprite>()->GetImgKey().compare("Hero_Idle") != 0)
				{
					_body->GetComponent<Sprite>()->SetImgName("Hero_Idle");
					_arms->GetComponent<Sprite>()->Start();
				}
			}
			else if (_armsAngle < 0.6f)
			{
				if (_body->GetComponent<Sprite>()->GetImgKey().compare("Hero_Idle_d") != 0)
				{
					_body->GetComponent<Sprite>()->SetImgName("Hero_Idle_d");
					_arms->GetComponent<Sprite>()->Start();
				}
			}
		}
	}
}

void Player::Move()
{
	if (KEYMANAGER->isStayKeyDown('D'))
	{
		if (_cw == CursorWhere::Left)
		{
			if (_legs->GetComponent<Sprite>()->GetImgKey().compare("Legs_Run") == 0 ||
				_legs->GetComponent<Sprite>()->GetImgKey().compare("Legs_Idle") == 0)
				_legs->GetComponent<Sprite>()->SetImgName("Legs_Run_Back");
		}
		else if (_cw == CursorWhere::Right)
		{
			if (_legs->GetComponent<Sprite>()->GetImgKey().compare("Legs_Run_Back") == 0 ||
				_legs->GetComponent<Sprite>()->GetImgKey().compare("Legs_Idle") == 0)
				_legs->GetComponent<Sprite>()->SetImgName("Legs_Run");
		}

		_dir = Dir::Right;
		_physics->ApplyForce(Vector2::b2Down);

		_trans->SetPos(_trans->GetPos() + Vector2(cosf((int)_dir * Deg2Rad), -sinf((int)_dir * Deg2Rad)) *
			_ability->GetSpeed() * TIMEMANAGER->getElapsedTime());
		_physics->SetBodyPosition();
	}
	else if (KEYMANAGER->isStayKeyDown('A'))
	{
		if (_cw == CursorWhere::Left)
		{
			if (_legs->GetComponent<Sprite>()->GetImgKey().compare("Legs_Run_Back") == 0 ||
				_legs->GetComponent<Sprite>()->GetImgKey().compare("Legs_Idle") == 0)
				_legs->GetComponent<Sprite>()->SetImgName("Legs_Run");
		}
		else if (_cw == CursorWhere::Right)
		{
			if (_legs->GetComponent<Sprite>()->GetImgKey().compare("Legs_Run") == 0 ||
				_legs->GetComponent<Sprite>()->GetImgKey().compare("Legs_Idle") == 0)
				_legs->GetComponent<Sprite>()->SetImgName("Legs_Run_Back");
		}

		_dir = Dir::Left;
		_physics->ApplyForce(Vector2::b2Down);

		_trans->SetPos(_trans->GetPos() + Vector2(cosf((int)_dir * Deg2Rad), -sinf((int)_dir * Deg2Rad)) *
			_ability->GetSpeed() * TIMEMANAGER->getElapsedTime());
		_physics->SetBodyPosition();
	}

	if (!KEYMANAGER->isStayKeyDown('A') && !KEYMANAGER->isStayKeyDown('D'))
	{
		if (_legs->GetComponent<Sprite>()->GetImgKey().compare("Legs_Run") == 0 ||
			_legs->GetComponent<Sprite>()->GetImgKey().compare("Legs_Run_Back") == 0)
			_legs->GetComponent<Sprite>()->SetImgName("Legs_Idle");
	}
}

void Player::Non_CombatModeMove()
{
	if (KEYMANAGER->isStayKeyDown('D'))
	{
		_sprite->SetFlipX(false);
		_dir = Dir::Right;

		_physics->GetBody()->SetLinearVelocity(Vector2::b2Right * _ability->GetSpeed() * 0.7f * TIMEMANAGER->getElapsedTime());
	}
	else if (KEYMANAGER->isStayKeyDown('A'))
	{
		_sprite->SetFlipX(true);
		_dir = Dir::Left;

		_physics->GetBody()->SetLinearVelocity(Vector2::b2Left * _ability->GetSpeed() * 0.7f * TIMEMANAGER->getElapsedTime());
	}
}

void Player::Parse()
{
	ifstream file("PlayerInfo.json");
	string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	json j = json::parse(content);

	for (json::iterator it = j["Inven"].begin(); it != j["Inven"].end(); ++it)
	{
		_mInven.insert(make_pair(j["Inven"][it.key()]["name"].get<string>(),
			new Item(j["Inven"][it.key()]["name"].get<string>(),
				j["Inven"][it.key()]["type"].get<ItemType>(),
				j["Inven"][it.key()]["count"].get<int>(),
				j["Inven"][it.key()]["price"].get<int>())));
	}

	_reminingAmmo = _mInven.find("PISTOL AMMO")->second->count;
	_magazine = j["magazine"];
	_ammo = j["ammo"];
	_isBattery = j["isBattery"];
	_prevScene = j["prevScene"];
	_money = j["money"];

	file.close();
}

void Player::AddItem(string item, int count)
{
	mInvenIter it = _mInven.find(item);

	if (it != _mInven.end())
	{
		_mInven[item]->count += count;
		if (_mInven[item]->type == ItemType::Ammo) _reminingAmmo = _mInven[item]->count;
		return;
	}
	else
	{
		_money += count;
		return;
	}
}
