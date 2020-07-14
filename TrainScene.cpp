#include "stdafx.h"
#include "TrainScene.h"
#include "Player.h"
#include "Train.h"
#include "CrossImage.h"
#include "GroundScene.h"
#include "FadeInAndOut.h"
#include "Item.h"
#include "NPCManager.h"

void TrainScene::Init()
{
	Scene::Init();

	_name = "Train";

	GRAPHICMANAGER->AddImage("btnSelect", L"btnSelect.png", 1, 2);
	GRAPHICMANAGER->AddImage("sky_2", L"Resource/Map/sky_2.png");
	GRAPHICMANAGER->AddImage("ground3", L"Resource/Map/ground3.png");
	GRAPHICMANAGER->AddImage("ground4", L"Resource/Map/ground4.png");
	GRAPHICMANAGER->AddImage("grass", L"Resource/Map/grass.png");
	GRAPHICMANAGER->AddImage("rails_2", L"Resource/Train/rails_2.png");
	GRAPHICMANAGER->AddImage("FoodBox", L"Resource/UI/Food.png");
	GRAPHICMANAGER->AddImage("MedkitBox", L"Resource/UI/Medkit.png");

	GRAPHICMANAGER->AddImage("ui_close", L"Resource/UI/ui_close.png", 2, 1);

	SCENEMANAGER->addScene("ground", new GroundScene);

	SOUNDMANAGER->addSound("Train Sound", "Resource/Sound/Train Sound.mp3", true, true);

	_train = Object::CreateObject<Train>();
	_train->GetTrans()->SetPos(WINSIZEX / 2, WINSIZEY - 280);
	_train->Init();
	_train->GetSprite()->SetDepth(1);
	_train->GetWheelsSprite()->SetScale(Vector2(1.1f, 1.1f));
	_train->GetWheelsSprite()->SetDepth(2);
	
	_player = Object::CreateObject<Player>();
	_player->GetTrans()->SetScale(28, 68);
	_player->Init();
	_player->GetTrans()->SetPos(WINSIZEX / 2, 457);
	_player->GetSprite()->SetDepth(5);
	_player->GetSprite()->SetScale(Vector2(1.2f, 1.2f));

	_player->GetPhysicsBody()->SetBodyPosition();

	_npcMgr = new NPCManager;
	_npcMgr->Init();
	_npcMgr->SetNPC(NPC_Type::Alex, _train->GetChairPosition(0));
	_npcMgr->SetNPC(NPC_Type::Javis, _train->GetChairPosition(4));

	_cursorImg = new Graphic;
	_cursorImg->Init(Direct2D::GetInstance()->CreateBitmap(L"Resource/UI/cursor.png"), "cursor", L"Resource/UI/cursor.png");

	_boxImg = new Graphic;
	_boxImg = GRAPHICMANAGER->FindImage("None");

	_ui = new UI;
	_ui->Init();
	_ui->LinkedNPCManager(_npcMgr);

	Background();

	_timer = 0.f;
	_txtAlpha = 0.f;

	_fade = new FadeInAndOut;
	_fade->FadeInStart();

	CAMERA->SetPos(Vector2(0, 0));

	SOUNDMANAGER->play("Train Sound");

	_player->SetIsBattery(false);
}

void TrainScene::Release()
{
	SAFE_OBJECT_RELEASE(_cursorImg);
	SAFE_OBJECT_RELEASE(_sky);
	SAFE_OBJECT_RELEASE(_ground3);
	SAFE_OBJECT_RELEASE(_ground4);
	SAFE_OBJECT_RELEASE(_ui);
	SAFE_OBJECT_RELEASE(_npcMgr);

	SAFE_DELETE(_cursorImg);
	SAFE_DELETE(_sky);
	SAFE_DELETE(_ground3);
	SAFE_DELETE(_ground4);
	SAFE_DELETE(_fade);
	SAFE_DELETE(_ui);
	SAFE_DELETE(_npcMgr);

	_player->SetCollisionType(ColType::None);

	Scene::Release();
}

void TrainScene::Update()
{
	_fade->Update();

	_ui->Update();

	Scene::Update();

	_npcMgr->Update();

	_sky->Update();
	_ground4->Update();
	_ground3->Update();

	_grassX -= GRASS_SPEED * TIMEMANAGER->getElapsedTime();
	_grassX_2 -= GRASS_SPEED * TIMEMANAGER->getElapsedTime();
	if (_grassX < -(WINSIZEX + 200)) _grassX = _grassX_2 + GRAPHICMANAGER->FindImage("grass")->GetFrameWidth();
	if (_grassX_2 < -(WINSIZEX + 200)) _grassX_2 = _grassX + GRAPHICMANAGER->FindImage("grass")->GetFrameWidth();

	if (KEYMANAGER->isOnceKeyDown('2')) SCENEMANAGER->changeScene("ground");

	if (_timer < ARRIVE_TIEM) _timer += 1.f * TIMEMANAGER->getElapsedTime();
	else _train->ArriveAtTheStation();

	if (KEYMANAGER->isOnceKeyDown('E'))
	{
		switch (_player->GetCollisionType())
		{
			case ColType::TrainStop: _fade->FadeOutStart(); break;
			case ColType::FoodBox:
			{
				if (_player->GetIsHoldItem())
				{
					_boxImg = GRAPHICMANAGER->FindImage("None");
					_player->SetIsHoldItem(false);
					_player->GetInventory().find("FOOD")->second->count += 1;
					_player->GetSprite()->SetImgName("train_idle");
					_train->GetMedkitObject()->GetPhysics()->SetBodyActive(true);
				}
				else
				{
					if (_player->GetInventory().find("FOOD")->second->count <= 0) break;

					_boxImg = GRAPHICMANAGER->FindImage("FoodBox");
					_player->SetIsHoldItem(true);
					_player->GetInventory().find("FOOD")->second->count -= 1;
					_player->GetSprite()->SetImgName("train_idle_holdItem");
					_train->GetMedkitObject()->GetPhysics()->SetBodyActive(false);
					UI::Text("FOOD", _train->GetFoodObject()->GetTrans()->GetPos() + Vector2::up * 47, Vector2(100, 20), ColorF::Azure);
				}
			}
			break;
			case ColType::MedkitBox:
			{
				if (_player->GetIsHoldItem())
				{
					_boxImg = GRAPHICMANAGER->FindImage("None");
					_player->SetIsHoldItem(false);
					_player->GetInventory().find("MEDKIT")->second->count += 1;
					_player->GetSprite()->SetImgName("train_idle");
					_train->GetFoodObject()->GetPhysics()->SetBodyActive(true);
				}
				else
				{
					if (_player->GetInventory().find("MEDKIT")->second->count <= 0) break;

					_boxImg = GRAPHICMANAGER->FindImage("MedkitBox");
					_player->SetIsHoldItem(true);
					_player->GetInventory().find("MEDKIT")->second->count -= 1;
					_player->GetSprite()->SetImgName("train_idle_holdItem");
					_train->GetFoodObject()->GetPhysics()->SetBodyActive(false);
					UI::Text("MEDKIT", _train->GetMedkitObject()->GetTrans()->GetPos() + Vector2::up * 47, Vector2(100, 20), ColorF::Azure);
				}
			}
			break;
			case ColType::Vent:
				_ui->SetShowTrainPart(TrainPart::Vent);
				_player->SetIsMove(false);
				break;
			case ColType::NPC:
				if (_player->GetIsHoldItem())
				{
					if (_boxImg->GetImageKey() == "FoodBox")
					{
						_npcMgr->GetNPC().find(_npcMgr->FindNPC(_ui->GetNpcName())->GetName())->second->EatFood(EAT_FOOD);
						_train->GetFoodObject()->GetPhysics()->SetBodyActive(true);
						_train->GetMedkitObject()->GetPhysics()->SetBodyActive(true);
					}
					if (_boxImg->GetImageKey() == "MedkitBox")
					{
						_npcMgr->GetNPC().find(_npcMgr->FindNPC(_ui->GetNpcName())->GetName())->second->HealHP(HEAL_HP);
						_train->GetFoodObject()->GetPhysics()->SetBodyActive(true);
						_train->GetMedkitObject()->GetPhysics()->SetBodyActive(true);
					}
					_player->SetIsHoldItem(false);
					_player->GetSprite()->SetImgName("train_idle");
					_boxImg = _boxImg = GRAPHICMANAGER->FindImage("None");
				}
				break;
		}
	}

#pragma region Food/Medkit Text
	if (_player->GetCollisionType() == ColType::FoodBox)
	{
		_txt = "FOOD " + to_string(_player->GetInventory().find("FOOD")->second->count);
		_txtPos = _train->GetFoodObject()->GetTrans()->GetPos() + Vector2(-100.f, -150.f);
		_txtAlpha = 1.f;
	}
	else if (_player->GetCollisionType() == ColType::MedkitBox)
	{
		_txt = "MEDKIT " + to_string(_player->GetInventory().find("MEDKIT")->second->count);
		_txtPos = _train->GetMedkitObject()->GetTrans()->GetPos() + Vector2(-100.f, -150.f);
		_txtAlpha = 1.f;
	}
	else
	{
		if (_txtAlpha > 0.f) _txtAlpha -= 5.f * TIMEMANAGER->getElapsedTime();
	}
#pragma endregion

	if (_fade->GetIsFadeOutEnd())
	{
		SCENEMANAGER->changeScene("ground");
		return;
	}
}

void TrainScene::Render()
{
	_sky->Render();
	_ground4->Render();
	_ground3->Render();
	GRAPHICMANAGER->DrawImage("rails_2", Vector2(0, WINSIZEY - 167), 1.0f, false, PIVOT::LEFT_TOP);

	Scene::Render();

	_boxImg->Render(_player->GetTrans()->GetPos() + Vector2::down * 4);
	
	GRAPHICMANAGER->DrawImage("grass", Vector2(_grassX, WINSIZEY - 280), 1.0f, false, PIVOT::LEFT_TOP);
	GRAPHICMANAGER->DrawImage("grass", Vector2(_grassX_2, WINSIZEY - 280), 1.0f, false, PIVOT::LEFT_TOP);

	_ui->Render();

	if(_txtAlpha > 0.f)
		GRAPHICMANAGER->Text(_txtPos, _txt.c_str(), 30, 200, 30, ColorF(ColorF::Azure, _txtAlpha), TextPivot::CENTER, L"Munro Small", true);

	_cursorImg->Render(MOUSEPOINTER->GetMouseWorldPosition().x, MOUSEPOINTER->GetMouseWorldPosition().y);

	_fade->Render();
}

void TrainScene::Background()
{
	_sky = new CrossImage;
	_sky->Init("sky_2", Vector2(0, 0), Vector2(GRAPHICMANAGER->FindImage("sky_2")->GetFrameWidth() - 1.f, 0), 10.f);

	_ground3 = new CrossImage;
	_ground3->Init("ground3", Vector2(0, WINSIZEY / 2 + 10), Vector2(GRAPHICMANAGER->FindImage("ground3")->GetFrameWidth() - 1.f, WINSIZEY / 2 + 10), 700.f);

	_ground4 = new CrossImage;
	_ground4->Init("ground4", Vector2(0, WINSIZEY / 2 - 30), Vector2(GRAPHICMANAGER->FindImage("ground4")->GetFrameWidth() - 1.f, WINSIZEY / 2 - 30), 200.f);

	_grassX = WINSIZEX + 10;
	_grassX_2 = _grassX + GRAPHICMANAGER->FindImage("grass")->GetFrameWidth();

	Object* leftWall = Object::CreateObject<Object>();
	leftWall->GetTrans()->SetPos(58, 464);
	leftWall->GetTrans()->SetScale(TILE_WIDTH, 70);

	PhysicsBody* p = leftWall->AddComponent<PhysicsBody>();
	p->Init(BodyType::STATIC, 10.f);

	Object* rightWall = Object::CreateObject<Object>();
	rightWall->GetTrans()->SetPos(WINSIZEX - 180, 464);
	rightWall->GetTrans()->SetScale(TILE_WIDTH, 70);

	p = rightWall->AddComponent<PhysicsBody>();
	p->Init(BodyType::STATIC, 10.f);
}