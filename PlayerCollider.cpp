#include "stdafx.h"
#include "PlayerCollider.h"
#include "Tile.h"
#include "Player.h"
#include "EnemyNormal.h"
#include "UndergroundScene.h"
#include "Item.h"
#include "FoodObject.h"
#include "MedkitObject.h"
#include "VentObject.h"
#include "Train.h"

void PlayerCollider::CollisionBegin(void* obj)
{
	Object* to = (Object*)obj;
	Player* me = (Player*)_object;

	if (to->GetName().compare("stops") == 0)
	{
		Train* train = (Train*)to->GetParent();
		Sprite* s = train->GetStops()->GetComponent<Sprite>();
		s->SetImgName(train->GetStops()->GetName() + "_White");
		s->SetPosition(Vector2(WINSIZEX / 2 + 330, 423.f));
		me->SetCollisionType(ColType::TrainStop);
	}

	if (to->GetName().compare("food_box") == 0)
	{
		FoodObject* food = (FoodObject*)obj;
		me->SetCollisionType(ColType::FoodBox);
		food->GetSprite()->SetImgName(food->GetName() + "_White");
	}
	if (to->GetName().compare("medkit_box") == 0)
	{
		MedkitObject* medkit = (MedkitObject*)obj;
		me->SetCollisionType(ColType::MedkitBox);
		medkit->GetSprite()->SetImgName(medkit->GetName() + "_White");
	}
	if (to->GetName().compare("panel_2") == 0)
	{
		VentObject* vent = (VentObject*)obj;
		me->SetCollisionType(ColType::Vent);
		vent->GetSprite()->SetImgName(vent->GetName() + "_White");
	}

	if (to->GetTag().compare("Tile") == 0)
	{
		Tile* tile = (Tile*)obj;

		if (tile->GetAttribute() == TAttribute::INTERACTION)
		{
			if (SCENEMANAGER->GetNowScene()->GetName() == "underground")
			{
				UndergroundScene* scene = (UndergroundScene*)SCENEMANAGER->GetNowScene();
				scene->SetTileIndex(tile->GetIndex());
			}

			tile->GetBackSprite()->SetImgName(tile->GetImgName() + "_White");
			me->SetCollisionType(ColType::Interaction);
		}
		if (tile->GetAttribute() == TAttribute::DEAD_BODY)
		{
			if (SCENEMANAGER->GetNowScene()->GetName() == "underground")
			{
				UndergroundScene* scene = (UndergroundScene*)SCENEMANAGER->GetNowScene();
				scene->SetTileIndex(tile->GetIndex());
			}

			tile->GetBackSprite()->SetImgName(tile->GetImgName() + "_White");
			me->SetCollisionType(ColType::Dead_Body);
		}
		if (tile->GetAttribute() == TAttribute::DOOR)
		{
			if (SCENEMANAGER->GetNowScene()->GetName() == "underground")
			{
				UndergroundScene* scene = (UndergroundScene*)SCENEMANAGER->GetNowScene();
				scene->SetTileIndex(tile->GetIndex());
			}

			if(tile->GetImgName() == tile->GetSprite()->GetImgKey())
				tile->GetBackSprite()->SetImgName(tile->GetImgName() + "_White");
			me->SetCollisionType(ColType::Door);
		}
		if (tile->GetAttribute() == TAttribute::KEY)
		{
			if (SCENEMANAGER->GetNowScene()->GetName() == "underground")
			{
				UndergroundScene* scene = (UndergroundScene*)SCENEMANAGER->GetNowScene();
				scene->SetTileIndex(tile->GetIndex());
			}

			tile->GetBackSprite()->SetImgName(tile->GetImgName() + "_White");
			me->SetCollisionType(ColType::Key);
		}
		if (tile->GetAttribute() == TAttribute::BATTERY)
		{
			if (SCENEMANAGER->GetNowScene()->GetName() == "underground")
			{
				UndergroundScene* scene = (UndergroundScene*)SCENEMANAGER->GetNowScene();
				scene->SetTileIndex(tile->GetIndex());
			}

			tile->GetBackSprite()->SetImgName(tile->GetImgName() + "_White");
			me->SetCollisionType(ColType::Battery);
		}

		/*if (tile->GetImgName().compare("Wall_RB") == 0)
		{
			me->GetLegs()->GetTrans()->SetRotateToRadian(7 * PI / 4);
			me->GetBody()->GetTrans()->SetRotateToRadian(7 * PI / 4);
		}*/
	}
}

void PlayerCollider::CollisionPreSolve(void* obj)
{
	Object* to = (Object*)obj;
	Player* me = (Player*)_object;

	if (to->GetTag().compare("Tile") == 0)
	{
		Tile* tile = (Tile*)obj;

		if (tile->GetAttribute() == TAttribute::DOOR)
		{
			if (tile->GetImgName() == "Door 1")
			{
				if (KEYMANAGER->isOnceKeyDown('E'))
				{
					SOUNDMANAGER->play("Door Open 2");

					me->SetCollisionType(ColType::None);

					tile->SetAttribute(TAttribute::NONE);
					tile->GetBackSprite()->SetImgName("None");
					tile->GetSprite()->SetImgName(tile->GetImgName() + "_Open");
					tile->GetPhysics()->SetSensor(true);
				}
			}
			else if (tile->GetImgName() == "Toilet")
			{
				if (KEYMANAGER->isOnceKeyDown('E'))
				{
					if (me->GetInventory().find("GOLD KEY")->second->count < 1) return;

					SOUNDMANAGER->play("Door Open 2");

					me->GetInventory().find("GOLD KEY")->second->count -= 1;

					tile->GetBackSprite()->SetImgName("None");
					tile->GetSprite()->SetImgName(tile->GetImgName() + "_Open");
					tile->GetPhysics()->SetSensor(true);
				}
			}
		}
	}
}

void PlayerCollider::CollisionEnd(void* obj)
{
	Object* to = (Object*)obj;
	Player* me = (Player*)_object;

	if (to->GetName().compare("stops") == 0)
	{
		Train* train = (Train*)to->GetParent();
		Sprite* s = train->GetStops()->GetComponent<Sprite>();
		s->SetImgName(train->GetStops()->GetName());
		s->SetFrameX(s->GetMaxFrameX());
		s->SetPosition(Vector2(WINSIZEX / 2 + 330, 423.f));
		me->SetCollisionType(ColType::None);
	}
	if (to->GetName().compare("food_box") == 0)
	{
		FoodObject* food = (FoodObject*)obj;
		me->SetCollisionType(ColType::None);
		food->GetSprite()->SetImgName(food->GetName());
	}
	if (to->GetName().compare("medkit_box") == 0)
	{
		MedkitObject* medkit = (MedkitObject*)obj;
		me->SetCollisionType(ColType::None);
		medkit->GetSprite()->SetImgName(medkit->GetName());
	}
	if (to->GetName().compare("panel_2") == 0)
	{
		VentObject* vent = (VentObject*)obj;
		me->SetCollisionType(ColType::None);
		vent->GetSprite()->SetImgName(vent->GetName());
	}

	if (to->GetTag().compare("Tile") == 0)
	{
		Tile* tile = (Tile*)obj;

		if (tile->GetAttribute() == TAttribute::DOOR || tile->GetAttribute() == TAttribute::DEAD_BODY ||
			tile->GetAttribute() == TAttribute::INTERACTION || tile->GetAttribute() == TAttribute::KEY ||
			tile->GetAttribute() == TAttribute::BATTERY)
		{
			if (SCENEMANAGER->GetNowScene()->GetName() == "underground")
			{
				UndergroundScene* scene = (UndergroundScene*)SCENEMANAGER->GetNowScene();
				scene->SetTileIndex(-1);
			}

			tile->GetBackSprite()->SetImgName("None");
			me->SetCollisionType(ColType::None);
		}
	}
}
