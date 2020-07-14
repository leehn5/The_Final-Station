#include "stdafx.h"
#include "UndergroundScene.h"
#include "Player.h"
#include "Tile.h"
#include <filesystem>
#include "Maptool.h"
#include "StartScene.h"
#include "UI.h"
#include "EnemyManager.h"
#include "PlayerState.h"
#include "ItemManager.h"
#include "Item.h"
#include "GroundScene.h"

void UndergroundScene::Init()
{
	Scene::Init();

	ShowCursor(false);

	GRAPHICMANAGER->AddImage("test", L"Resource/UI/test2.png");

	SOUNDMANAGER->addSound("Soundtrack Porth Wen", "Resource/Sound/Soundtrack Porth Wen.mp3", true, true);
	SOUNDMANAGER->addSound("Pick up 2", "Resource/Sound/Pick up 2.mp3", false, false);

	_name = "underground";

	_player = Object::CreateObject<Player>();
	_player->Init();
	_player->GetTrans()->SetPos(WINSIZEX / 2 - 500, 325);
	_player->GetPhysicsBody()->SetBodyPosition();

	_enemyMgr = new EnemyManager;
	_enemyMgr->Init();

	AddImage();
	MapLoad();

	_ui = new UI;
	_ui->Init();

	_cursorImg = new Graphic;
	_cursorImg->Init(Direct2D::GetInstance()->CreateBitmap(L"Resource/UI/cursor.png"), "cursor", L"Resource/UI/cursor.png");

	SCENEMANAGER->addScene("maptool", new Maptool);
	SCENEMANAGER->addScene("start", new StartScene);
	SCENEMANAGER->addScene("underground", new UndergroundScene);
	SCENEMANAGER->addScene("ground", new GroundScene);

	_isShowRect = false;
	_tileIndex = -1;

	_fade = new FadeInAndOut;
	_fade->FadeInStart();

	_timer = 0.f;

	SCENEMANAGER->GetNowScene()->GetWorld()->SetGravity(b2Vec2(0.0f, 15.0f));

	SOUNDMANAGER->play("Soundtrack Porth Wen");
}

void UndergroundScene::Release()
{
	_vTiles.clear();

	SAFE_OBJECT_RELEASE(_ui);
	SAFE_OBJECT_RELEASE(_enemyMgr);
	SAFE_OBJECT_RELEASE(_cursorImg);

	SAFE_DELETE(_ui);
	SAFE_DELETE(_enemyMgr);
	SAFE_DELETE(_cursorImg);
	SAFE_DELETE(_fade);

	SOUNDMANAGER->stop("Soundtrack Porth Wen");

	Scene::Release();
}

void UndergroundScene::Update()
{
	_fade->Update();

	CAMERA->SetPosition(_player->GetTrans()->GetPos());
	//CAMERA->Control();
	
	Scene::Update();

	_ui->Update();

	// tile draw rect
	if (KEYMANAGER->isOnceKeyUp(VK_F1))
	{
		_isShowRect = !_isShowRect;

		for (Tile* t : _vTiles)
		{
			t->GetSprite()->SetShowRect(_isShowRect);
			t->GetTileObject()->GetSprite()->SetShowRect(_isShowRect);
		}
	}

	if (_tileIndex != -1)
	{
		if (KEYMANAGER->isOnceKeyDown('E'))
		{
			if (_player->GetCollisionType() == ColType::Key)
			{
				SOUNDMANAGER->play("Pick up 2");
				_vTiles[_tileIndex]->GetTileObject()->SetAttribute(TAttribute::NONE);
				_vTiles[_tileIndex]->GetTileObject()->GetBackSprite()->SetImgName("None");
				_vTiles[_tileIndex]->GetTileObject()->GetPhysics()->SetBodyActive(false);

				ItemManager* t = _vTiles[_tileIndex]->GetTileObject()->AddComponent<ItemManager>();
				t->AddItem(0);
				_player->AddItem(t->GetItemList()[0][0]->name, t->GetItemList()[0][0]->count);

				UI::Text(t->GetItemList()[0][0]->name + " X" + to_string(t->GetItemList()[0][0]->count), _player->GetTrans()->GetPos() + Vector2::up * 30, Vector2(300, 30), ColorF::Azure);

				_tileIndex = -1;
			}
			else if (_player->GetCollisionType() == ColType::Door)
			{
				if (_vTiles[_tileIndex]->GetTileObject()->GetSprite()->GetImgKey() != _vTiles[_tileIndex]->GetTileObject()->GetImgName())
					_fade->FadeOutStart();
			}
			else if (_player->GetCollisionType() == ColType::Battery)
			{
				SOUNDMANAGER->play("Pick up 2");

				_vTiles[_tileIndex]->GetTileObject()->SetAttribute(TAttribute::NONE);
				_vTiles[_tileIndex]->GetTileObject()->GetBackSprite()->SetImgName("None");
				_vTiles[_tileIndex]->GetTileObject()->GetPhysics()->SetBodyActive(false);
				_vTiles[_tileIndex]->GetTileObject()->SetIsActive(false);

				_player->SetIsBattery(true);
				
				ItemManager* t = _vTiles[_tileIndex]->GetTileObject()->AddComponent<ItemManager>();
				t->AddItem(1);

				UI::Text(t->GetItemList()[0][0]->name + " X" + to_string(t->GetItemList()[0][0]->count), 
					_player->GetTrans()->GetPos() + Vector2::up * 30, Vector2(300, 30), ColorF::Azure);

				_tileIndex = -1;
			}

			else if (_player->GetCollisionType() == ColType::Interaction || _player->GetCollisionType() == ColType::Dead_Body)
			{
				SOUNDMANAGER->play("Pick up 2");

				if (_player->GetCollisionType() == ColType::Interaction)
				{
					_vTiles[_tileIndex]->GetTileObject()->GetSprite()->SetImgName(_vTiles[_tileIndex]->GetTileObject()->GetImgName() + "_Open");
					_vTiles[_tileIndex]->GetTileObject()->GetSprite()->SetPosition(_vTiles[_tileIndex]->GetTileObject()->GetSprite()->GetPosition() + Vector2(4, 0));
				}
				_vTiles[_tileIndex]->GetTileObject()->SetAttribute(TAttribute::NONE);
				_vTiles[_tileIndex]->GetTileObject()->GetBackSprite()->SetImgName("None");
				_vTiles[_tileIndex]->GetTileObject()->GetPhysics()->SetBodyActive(false);

				ItemManager* t = _vTiles[_tileIndex]->GetTileObject()->AddComponent<ItemManager>();
				t->RandomAddItem();

				for (int i = 0; i < t->GetItemList()[0].size(); ++i)
				{
					t->GetItemList()[0][i]->count = RND->getFromIntTo(1, 8);
					_player->AddItem(t->GetItemList()[0][i]->name, t->GetItemList()[0][i]->count);

					//cout << "name : " << t->GetItemList()[0][i]->name << " count : " << t->GetItemList()[0][i]->count << endl;
				}
				if (t->GetItemList()[0].size() == 1)
					UI::Text(t->GetItemList()[0][0]->name + " X" + to_string(t->GetItemList()[0][0]->count), _player->GetTrans()->GetPos() + Vector2::up * 30, Vector2(300, 30), ColorF::Azure);

				else if (t->GetItemList()[0].size() == 2)
					UI::Text(t->GetItemList()[0][0]->name + " X" + to_string(t->GetItemList()[0][0]->count) + "\n" +
						t->GetItemList()[0][1]->name + " X" + to_string(t->GetItemList()[0][1]->count),
						_player->GetTrans()->GetPos() + Vector2::up * 60, Vector2(300, 60), ColorF::Azure);

				else if (t->GetItemList()[0].size() == 3)
					UI::Text(t->GetItemList()[0][0]->name + " X" + to_string(t->GetItemList()[0][0]->count) + "\n" +
						t->GetItemList()[0][1]->name + " X" + to_string(t->GetItemList()[0][1]->count) + "\n" +
						t->GetItemList()[0][2]->name + " X" + to_string(t->GetItemList()[0][2]->count),
						_player->GetTrans()->GetPos() + Vector2::up * 90, Vector2(300, 90), ColorF::Azure);
				
				_tileIndex = -1;
			}
		}
	}

	if (KEYMANAGER->isOnceKeyDown('M'))
	{
		SCENEMANAGER->changeScene("maptool");
		return;
	}
	if (KEYMANAGER->isOnceKeyDown('P'))
	{
		SCENEMANAGER->changeScene("start");
		return;
	}

	if (_player->GetState()->GetState() == "Dead")
	{
		_timer += 1.f * TIMEMANAGER->getElapsedTime();

		if (_timer > 4.5f) 
		{
			_timer = 0.f;
			_fade->FadeOutStart();
		}
	}

	if (_player->GetState()->GetState() == "Dead" && _fade->GetIsFadeOutEnd())
	{
		SCENEMANAGER->changeScene("underground");
		return;
	}
	else if (_player->GetState()->GetState() != "Dead" && _fade->GetIsFadeOutEnd())
	{
		SCENEMANAGER->changeScene("ground");
		return;
	}

	_enemyMgr->Update();
}

void UndergroundScene::Render()
{
	Scene::Render();

	GRAPHICMANAGER->DrawImage("test", _player->GetTrans()->GetPos(), 1.f, false);
	_ui->Render();

	_cursorImg->Render(MOUSEPOINTER->GetMouseWorldPosition().x, MOUSEPOINTER->GetMouseWorldPosition().y);

	_fade->Render();
}

void UndergroundScene::MapLoad()
{
	for (int i = 0; i < TILE_NUM_Y; ++i)
	{
		for (int j = 0; j < TILE_NUM_X; ++j)
		{
			Tile* tile = Object::CreateObject<Tile>();
			tile->Init();
			tile->GetTrans()->SetPos(j * TILE_WIDTH + (TILE_WIDTH / 2),
				i * TILE_HEIGHT + (TILE_HEIGHT / 2));
			tile->SetIdX(j);
			tile->SetIdY(i);
			tile->SetIndex(j + TILE_NUM_X * i);
			tile->GetSprite()->SetDepth(0);
			tile->SetAttribute(TAttribute::NONE);
			tile->SetTileObject(Object::CreateObject<Tile>());
			tile->GetTileObject()->Init();
			tile->GetTileObject()->GetTrans()->SetPos(tile->GetTrans()->GetPos());
			tile->GetTileObject()->GetSprite()->SetImgName("None");
			tile->GetTileObject()->SetIsActive(false);

			_vTiles.push_back(tile);
		}
	}

	ifstream inFile("test1.map");
	Vector2 pos;
	for (int i = 0; i < _vTiles.size(); ++i)
	{
		char buffer[512];
		inFile.getline(buffer, 512);
		_vTiles[i]->SetAttribute((TAttribute)atoi(buffer));

		inFile.getline(buffer, 512);
		_vTiles[i]->SetImgName(buffer);
		_vTiles[i]->GetSprite()->SetImgName(_vTiles[i]->GetImgName());

		inFile.getline(buffer, 512);
		pos.x = atof(buffer);
		inFile.getline(buffer, 512);
		pos.y = atof(buffer);
		_vTiles[i]->GetSprite()->SetPosition(pos);

		inFile.getline(buffer, 512);
		_vTiles[i]->GetSprite()->SetDepth(atoi(buffer));

		inFile.getline(buffer, 512);
		_vTiles[i]->SetIndex(atoi(buffer));

		// tile object setting
		inFile.getline(buffer, 512);
		_vTiles[i]->GetTileObject()->SetAttribute((TAttribute)atoi(buffer));

		inFile.getline(buffer, 512);
		_vTiles[i]->GetTileObject()->SetImgName(buffer);
		_vTiles[i]->GetTileObject()->GetSprite()->SetImgName(_vTiles[i]->GetTileObject()->GetImgName());

		inFile.getline(buffer, 512);
		pos.x = atof(buffer);

		inFile.getline(buffer, 512);
		pos.y = atof(buffer);

		_vTiles[i]->GetTileObject()->GetSprite()->SetPosition(pos);

		inFile.getline(buffer, 512);
		_vTiles[i]->GetTileObject()->GetSprite()->SetFrameX(atoi(buffer));

		inFile.getline(buffer, 512);
		_vTiles[i]->GetTileObject()->GetSprite()->SetFrameY(atoi(buffer));

		inFile.getline(buffer, 512);
		_vTiles[i]->GetTileObject()->GetSprite()->SetDepth(atoi(buffer));

		inFile.getline(buffer, 512);
		_vTiles[i]->GetTileObject()->SetIndex(atoi(buffer));

		inFile.getline(buffer, 512);
		_vTiles[i]->GetTileObject()->SetIsActive(atoi(buffer));

		ColorF color = ColorF::Blue;
		inFile.getline(buffer, 512);
		color.r = atof(buffer);
		inFile.getline(buffer, 512);
		color.g = atof(buffer);
		inFile.getline(buffer, 512);
		color.b = atof(buffer);
		inFile.getline(buffer, 512);
		color.a = atof(buffer);

		_vTiles[i]->GetTileObject()->GetSprite()->SetRectColor(color);

		inFile.getline(buffer, 512);
		_vTiles[i]->GetTileObject()->GetSprite()->SetFillRect(atoi(buffer));

		inFile.getline(buffer, 512);
		_vTiles[i]->SetTileParent(atoi(buffer));

		inFile.getline(buffer, 512);
		int size = atoi(buffer);

		for (int j = 0; j < size; ++j)
		{
			inFile.getline(buffer, 512);
			_vTiles[i]->AddTileChildren(_vTiles[atoi(buffer)]);
		}

		if (_vTiles[i]->GetAttribute() == TAttribute::WALL)
		{
			b2Filter b;
			b.categoryBits = CATEGORY_SCENERY;
			b.maskBits = MASK_SCENERY;

			if (_vTiles[i]->GetImgName().compare("Wall_RB") == 0)
			{
				_vTiles[i]->GetTrans()->SetPos(_vTiles[i]->GetTrans()->GetPos() + Vector2(14.5f, 14.5f));
				_vTiles[i]->GetTrans()->SetScale(_vTiles[i]->GetTrans()->GetScale() + Vector2(10.0f, 10.0f));
				_vTiles[i]->SetPhysics();
				_vTiles[i]->GetPhysics()->SetBodyPosition();
				_vTiles[i]->GetPhysics()->GetBody()->SetTransform(b2Vec2(_vTiles[i]->GetPhysics()->GetBody()->GetPosition().x, _vTiles[i]->GetPhysics()->GetBody()->GetPosition().y), PI / 4);
				_vTiles[i]->GetPhysics()->GetBody()->GetFixtureList()->SetFilterData(b);
			}
			else if (_vTiles[i]->GetImgName().compare("Wall_RT") == 0)
			{
				_vTiles[i]->GetTrans()->SetPos(_vTiles[i]->GetTrans()->GetPos() + Vector2(14.5f, -14.5f));
				_vTiles[i]->GetTrans()->SetScale(_vTiles[i]->GetTrans()->GetScale() + Vector2(10.0f, 10.0f));
				_vTiles[i]->SetPhysics();
				_vTiles[i]->GetPhysics()->SetBodyPosition();
				_vTiles[i]->GetPhysics()->GetBody()->SetTransform(b2Vec2(_vTiles[i]->GetPhysics()->GetBody()->GetPosition().x, _vTiles[i]->GetPhysics()->GetBody()->GetPosition().y), PI / 4);
				_vTiles[i]->GetPhysics()->GetBody()->GetFixtureList()->SetFilterData(b);
			}
			else if (_vTiles[i]->GetImgName().compare("Wall_LB") == 0)
			{
				_vTiles[i]->GetTrans()->SetPos(_vTiles[i]->GetTrans()->GetPos() + Vector2(-14.5f, 14.5f));
				_vTiles[i]->GetTrans()->SetScale(_vTiles[i]->GetTrans()->GetScale() + Vector2(10.0f, 10.0f));
				_vTiles[i]->SetPhysics();
				_vTiles[i]->GetPhysics()->SetBodyPosition();
				_vTiles[i]->GetPhysics()->GetBody()->SetTransform(b2Vec2(_vTiles[i]->GetPhysics()->GetBody()->GetPosition().x, _vTiles[i]->GetPhysics()->GetBody()->GetPosition().y), PI / 4);
				_vTiles[i]->GetPhysics()->GetBody()->GetFixtureList()->SetFilterData(b);
			}
			else if (_vTiles[i]->GetImgName().compare("Wall_LT") == 0)
			{
				_vTiles[i]->GetTrans()->SetPos(_vTiles[i]->GetTrans()->GetPos() + Vector2(-14.5f, -14.5f));
				_vTiles[i]->GetTrans()->SetScale(_vTiles[i]->GetTrans()->GetScale() + Vector2(10.0f, 10.0f));
				_vTiles[i]->SetPhysics();
				_vTiles[i]->GetPhysics()->SetBodyPosition();
				_vTiles[i]->GetPhysics()->GetBody()->SetTransform(b2Vec2(_vTiles[i]->GetPhysics()->GetBody()->GetPosition().x, _vTiles[i]->GetPhysics()->GetBody()->GetPosition().y), PI / 4);
				_vTiles[i]->GetPhysics()->GetBody()->GetFixtureList()->SetFilterData(b);
			}
			else
			{
				_vTiles[i]->SetPhysics();
				_vTiles[i]->GetPhysics()->SetBodyPosition();
				_vTiles[i]->GetPhysics()->GetBody()->GetFixtureList()->SetFilterData(b);
			}
		}

		if (_vTiles[i]->GetTileObject()->GetAttribute() == TAttribute::LADDER)
		{
			if (_vTiles[i]->GetTileParent() == -1)
			{
				int children = _vTiles[i]->GetTileChildren().size();
				Vector2 size = Vector2(TILE_WIDTH, (children + 1) * TILE_HEIGHT);
				Vector2 pos = _vTiles[i]->GetTrans()->GetPos() + Vector2(0.0f, (size.y / 2) - (TILE_HEIGHT / 2));
				_vTiles[i]->GetTileObject()->GetTrans()->SetScale(size);
				_vTiles[i]->GetTileObject()->GetTrans()->SetPos(pos);

				_vTiles[i]->GetTileObject()->SetPhysics();
				_vTiles[i]->GetTileObject()->GetPhysics()->SetBodyPosition();
				_vTiles[i]->GetTileObject()->GetPhysics()->SetSensor(true);
			}
		}

		if (_vTiles[i]->GetTileObject()->GetAttribute() == TAttribute::KEY)
		{
			_vTiles[i]->GetTileObject()->SetPhysics();
			_vTiles[i]->GetTileObject()->GetPhysics()->SetBodyPosition();
			_vTiles[i]->GetTileObject()->GetPhysics()->SetSensor(true);
		}

		if (_vTiles[i]->GetTileObject()->GetAttribute() == TAttribute::BATTERY)
		{
			_vTiles[i]->GetTileObject()->SetPhysics();
			_vTiles[i]->GetTileObject()->GetPhysics()->SetBodyPosition();
			_vTiles[i]->GetTileObject()->GetPhysics()->SetSensor(true);
		}

		if (_vTiles[i]->GetTileObject()->GetAttribute() == TAttribute::DOOR)
		{
			if (_vTiles[i]->GetTileParent() == -1)
			{
				Vector2 size;
				Vector2 pos;
				if (_vTiles[i]->GetTileChildren().size() >= 1)
				{
					size.y = abs(_vTiles[i]->GetTrans()->GetPos().y - _vTiles[_vTiles[i]->GetTileChildren()[0]]->GetTrans()->GetPos().y);

					if (size.y > 0.0f)
					{
						size = Vector2(TILE_WIDTH, TILE_HEIGHT * 2);
						pos = _vTiles[i]->GetTrans()->GetPos() + Vector2(0.f, -TILE_HEIGHT / 2);
					}

					_vTiles[i]->GetTileObject()->GetTrans()->SetScale(size);
					_vTiles[i]->GetTileObject()->GetTrans()->SetPos(pos);
				}

				_vTiles[i]->GetTileObject()->SetPhysics();
				_vTiles[i]->GetTileObject()->GetPhysics()->SetBodyPosition();
			}
		}

		if (_vTiles[i]->GetTileObject()->GetAttribute() == TAttribute::INTERACTION ||
			_vTiles[i]->GetTileObject()->GetAttribute() == TAttribute::DEAD_BODY)
		{
			if (_vTiles[i]->GetTileParent() == -1)
			{
				Vector2 size;
				Vector2 pos;
				if (_vTiles[i]->GetTileChildren().size() >= 1)
				{
					size.x = abs(_vTiles[i]->GetTrans()->GetPos().x - _vTiles[_vTiles[i]->GetTileChildren()[0]]->GetTrans()->GetPos().x);
					size.y = abs(_vTiles[i]->GetTrans()->GetPos().y - _vTiles[_vTiles[i]->GetTileChildren()[0]]->GetTrans()->GetPos().y);

					if (size.x <= 0.0f)
					{
						size = Vector2(TILE_WIDTH, TILE_HEIGHT * 2);
						pos = _vTiles[i]->GetTrans()->GetPos() + Vector2(0.0f, -(TILE_HEIGHT / 2));
					}
					else if (size.x > 0.0f)
					{
						size = Vector2(TILE_WIDTH * 2, TILE_HEIGHT);
						pos = _vTiles[i]->GetTrans()->GetPos() + Vector2(TILE_WIDTH / 2, 0.0f);
					}

					_vTiles[i]->GetTileObject()->GetTrans()->SetScale(size);
					_vTiles[i]->GetTileObject()->GetTrans()->SetPos(pos);
				}

				_vTiles[i]->GetTileObject()->SetPhysics();
				_vTiles[i]->GetTileObject()->GetPhysics()->SetBodyPosition();
				_vTiles[i]->GetTileObject()->GetPhysics()->SetSensor(true);
			}
		}

		if (_vTiles[i]->GetTileObject()->GetAttribute() == TAttribute::ENEMY)
		{
			if (_vTiles[i]->GetTileParent() == -1)
			{
				_enemyMgr->SetEnemy(_vTiles[i]->GetTrans()->GetPos());
				_vTiles[i]->GetTileObject()->SetImgName("None");
				_vTiles[i]->GetTileObject()->GetSprite()->SetImgName("None");
			}
			_vTiles[i]->GetTileObject()->GetSprite()->SetRectColor(ColorF::Blue);
			_vTiles[i]->GetTileObject()->GetSprite()->SetFillRect(false);
			_vTiles[i]->GetTileObject()->SetIsActive(false);
		}
	}
	inFile.close();

	//for (int i = 0; i < _vTiles.size(); )
	//{
	//	if (_vTiles[i]->GetImgName().compare("None") == 0)
	//	{
	//		_vTiles.erase(_vTiles.begin() + i);
	//		//_vTiles[i]->SetIsActive(false);
	//	}
	//	else i++;
	//}

	for (Tile* t : _vTiles)
	{
		if (t->GetImgName().compare("None") == 0)
		{
			t->SetIsActive(false);
		}
	}
}

void UndergroundScene::AddImage()
{
	DirectorySearch("Resource/Terrain/None/");
	DirectorySearch("Resource/Terrain/Wall/");
	DirectorySearch("Resource/Object/None/");
	DirectorySearch("Resource/Object/Door/");
	DirectorySearch("Resource/Object/Door_Open/");
	DirectorySearch("Resource/White/");
	DirectorySearch("Resource/Object/Interaction/");
	DirectorySearch("Resource/Object/Key/");
	DirectorySearch("Resource/Object/Battery/");
	DirectorySearch("Resource/Object/Dead_Body/");
	DirectorySearch("Resource/NPC/Maptool/");
	DirectorySearch("Resource/Enemy/Maptool/");
	DirectorySearch("Resource/Object/Ladder/", 1, 3);
}

void UndergroundScene::DirectorySearch(string folderPath, int maxFrameX, int maxFrameY)
{
	string imgKey;
	wstring path;

	for (auto d : filesystem::directory_iterator(folderPath))
	{
		string a = d.path().string();
		path.assign(a.begin(), a.end());

		imgKey = d.path().string().substr(strlen(folderPath.c_str()), d.path().string().size() - (strlen(folderPath.c_str()) + 4));
		GRAPHICMANAGER->AddImage(imgKey, path, maxFrameX, maxFrameY);
	}
}